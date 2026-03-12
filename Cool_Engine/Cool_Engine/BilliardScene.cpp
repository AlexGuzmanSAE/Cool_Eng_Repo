#include "BilliardScene.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "box2d/box2d.h"
#include <cmath>
#include <string>

BilliardScene& BilliardScene::instance()
{
    static BilliardScene inst;
    return inst;
}

void BilliardScene::Load()
{
    b2World_SetGravity(physics.worldId, { 0.0f, 0.0f });

    buildTable();
    rackBalls();
    placeCueBall({ TABLE_X + TABLE_W * 0.25f, TABLE_Y + TABLE_H * 0.5f });

    state = BilliardState::AIMING;
    score = 0;
	bindRaylib();
}

void BilliardScene::UnLoad()
{
    if (cueBall)
        b2DestroyBody(cueBall->getBodyId());

    for (auto& b : balls)
        b2DestroyBody(b->getBodyId());

    for (auto& w : walls)
        b2DestroyBody(w->getBodyId());

    cueBall = nullptr;
    balls.clear();
    walls.clear();
    pockets.clear();
}

void BilliardScene::buildTable()
{
    float cx = TABLE_X + TABLE_W * 0.5f;
    float cy = TABLE_Y + TABLE_H * 0.5f;

    auto top = physics.create_billiard_box("wall_top", "wall",
        { TABLE_W + WALL_T * 2.0f, WALL_T }, false,
        { cx, TABLE_Y - WALL_T * 0.5f });
    top->color = CUSHION_COLOR;
    walls.push_back(top);

    // bottom
    auto bot = physics.create_billiard_box("wall_bot", "wall",
        { TABLE_W + WALL_T * 2.0f, WALL_T }, false,
        { cx, TABLE_Y + TABLE_H + WALL_T * 0.5f });
    bot->color = CUSHION_COLOR;
    walls.push_back(bot);

    // left
    auto lft = physics.create_billiard_box("wall_left", "wall",
        { WALL_T, TABLE_H }, false,
        { TABLE_X - WALL_T * 0.5f, cy });
    lft->color = CUSHION_COLOR;
    walls.push_back(lft);

    // right
    auto rgt = physics.create_billiard_box("wall_right", "wall",
        { WALL_T, TABLE_H }, false,
        { TABLE_X + TABLE_W + WALL_T * 0.5f, cy });
    rgt->color = CUSHION_COLOR;
    walls.push_back(rgt);

    pockets = {
        { TABLE_X,                    TABLE_Y           },  // arriba-izq
        { TABLE_X + TABLE_W * 0.5f,  TABLE_Y           },  // arriba-centro
        { TABLE_X + TABLE_W,         TABLE_Y           },  // arriba-der
        { TABLE_X,                    TABLE_Y + TABLE_H },  // abajo-izq
        { TABLE_X + TABLE_W * 0.5f,  TABLE_Y + TABLE_H },  // abajo-centro
        { TABLE_X + TABLE_W,         TABLE_Y + TABLE_H },  // abajo-der
    };
}

void BilliardScene::rackBalls()
{
    const Color COLORS[15] = {
        YELLOW, BLUE, RED, PURPLE, ORANGE, DARKGREEN, MAROON,
        BLACK,                              // 8-ball
        YELLOW, BLUE, RED, PURPLE, ORANGE, DARKGREEN, MAROON
    };

    float tipX = TABLE_X + TABLE_W * 0.65f;
    float tipY = TABLE_Y + TABLE_H * 0.5f;
    float space = BALL_R * 2.05f; 

    int idx = 0;
    for (int row = 0; row < 5; row++)
    {
        for (int col = 0; col <= row; col++)
        {
            float bx = tipX + row * space;
            float by = tipY + (col - row * 0.5f) * space;

            auto ball = physics.make_billiard_circle(
                "ball_" + std::to_string(idx + 1), "ball",
                BALL_R, true, { bx, by });
            ball->color = COLORS[idx];
            balls.push_back(ball);
            idx++;
        }
    }
}

void BilliardScene::placeCueBall(Vector2 pos)
{
    cueBall = physics.make_billiard_circle("cue_ball", "cue", BALL_R, true, pos);
    cueBall->color = WHITE;
}

bool BilliardScene::allStopped() const
{
    constexpr float THRESHOLD = 0.5f;

    auto stopped = [&](const std::shared_ptr<PCircle>& b) {
        b2Vec2 v = b2Body_GetLinearVelocity(b->getBodyId());
        return (v.x * v.x + v.y * v.y) < THRESHOLD;
        };

    if (cueBall && !stopped(cueBall)) return false;
    for (auto& b : balls)
        if (!stopped(b)) return false;
    return true;
}

void BilliardScene::checkPockets()
{
    for (auto it = balls.begin(); it != balls.end(); )
    {
        bool pocketed = false;
        for (auto& p : pockets)
        {
            if (Vector2Distance((*it)->position, p) < POCKET_R + BALL_R * 0.5f)
            {
                b2DestroyBody((*it)->getBodyId());
                it = balls.erase(it);
                score++;
                pocketed = true;
                break;
            }
        }
        if (!pocketed) ++it;
    }

    if (cueBall)
    {
        for (auto& p : pockets)
        {
            if (Vector2Distance(cueBall->position, p) < POCKET_R + BALL_R * 0.5f)
            {
                b2DestroyBody(cueBall->getBodyId());
                cueBall = nullptr;
                state = BilliardState::BALL_IN_HAND;
                return;
            }
        }
    }
}

void BilliardScene::bindRaylib()
{
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
    sol::table rl = lua.create_named_table("rl");

    rl.set_function("draw_circle", [](float x, float y, float r, sol::optional<Color> c)
        {
            Color color = c.value_or(GREEN);
            Vector2 center = { x, y };
            DrawCircleV(center, r, color);
        });
    rl.set_function("mouse_pressed", &IsMouseButtonPressed);
    rl.set_function("mouse_x", &GetMouseX);
    rl.set_function("mouse_y", &GetMouseY);
    //rl.set_function("to_world", [&](float sx, float sy)
    //    {
    //        /*Vector2 world = GetScreenToWorld2D({sx, sy}, cam);
    //        return std::make_tuple(world.x, world.y);*/
    //    });
    rl.set_function("print", [](std::string message)
        {
            std::cout << "[LUA]: " << message << std::endl;
        });
    rl["green"] = GREEN;
    auto result = lua.script_file("assets/scripts/game_scene.lua");
    if (result.valid())
    {
        luaUpdate = lua["update"];
        luaDraw = lua["draw"];
        std::cout << "Script cargado y funciones vinculadas." << std::endl;
    }
    else
    {
        sol::error err = result;
        std::cerr << "Error al cargar el script: " << err.what() << std::endl;
    }
}

void BilliardScene::UpdateScene()
{
    if (luaUpdate)
    {
		//std::cout << "Llamando a luaUpdate..." << std::endl;
        luaUpdate();
    }
    float dt = GetFrameTime();

    physics.update(dt);

    for (auto& w : walls)   w->syncFromBody();
    for (auto& b : balls)   b->syncFromBody();
    if (cueBall)             cueBall->syncFromBody();

    if (state == BilliardState::BALLS_MOVING)
    {
        checkPockets();

        if (balls.empty()) { state = BilliardState::GAME_OVER; return; }
        if (!cueBall) { state = BilliardState::BALL_IN_HAND; return; }
        if (allStopped()) { state = BilliardState::AIMING; }
        return;
    }

    if (state == BilliardState::BALL_IN_HAND)
    {
        Vector2 mp = GetMousePosition();
        bool inTable = mp.x > TABLE_X + BALL_R && mp.x < TABLE_X + TABLE_W - BALL_R &&
            mp.y > TABLE_Y + BALL_R && mp.y < TABLE_Y + TABLE_H - BALL_R;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && inTable)
        {
            placeCueBall(mp);
            state = BilliardState::AIMING;
        }
        return;
    }

    if (state == BilliardState::AIMING && cueBall)
    {
        Vector2 mp = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (Vector2Distance(mp, cueBall->position) < BALL_R * 4.0f)
            {
                dragging = true;
                dragPos = mp;
            }
        }

        if (dragging && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            dragPos = mp;

        if (dragging && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            Vector2 diff = Vector2Subtract(cueBall->position, dragPos);
            float   dist = Vector2Length(diff);

            if (dist > 5.0f)
            {
                float   power = fminf(dist / MAX_DRAG, 1.0f);
                Vector2 dir = Vector2Normalize(diff);
                b2Vec2  impulse = { dir.x * power * MAX_IMPULSE,
                                    dir.y * power * MAX_IMPULSE };

                b2Body_ApplyLinearImpulseToCenter(cueBall->getBodyId(), impulse, true);
                state = BilliardState::BALLS_MOVING;
            }
            dragging = false;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            dragging = false;
    }
}

void BilliardScene::Draw()
{
    if (luaDraw)
    {
        luaDraw();
    }

    DrawRectangle((int)(TABLE_X - WALL_T - 6), (int)(TABLE_Y - WALL_T - 6),
        (int)(TABLE_W + (WALL_T + 6) * 2), (int)(TABLE_H + (WALL_T + 6) * 2),
        TABLE_BORDER);

    DrawRectangle((int)TABLE_X, (int)TABLE_Y,
        (int)TABLE_W, (int)TABLE_H, FELT_COLOR);

    float lx = TABLE_X + TABLE_W * 0.25f;
    DrawLine((int)lx, (int)TABLE_Y, (int)lx, (int)(TABLE_Y + TABLE_H),
        Fade(WHITE, 0.15f));

    DrawCircle((int)(TABLE_X + TABLE_W * 0.65f),
        (int)(TABLE_Y + TABLE_H * 0.5f),
        3, Fade(WHITE, 0.3f));

    for (auto& p : pockets)
        DrawCircleV(p, POCKET_R, BLACK);

    for (auto& w : walls)
        w->draw();

    for (auto& b : balls)
        b->draw();

    if (cueBall)
        cueBall->draw();

    drawAimingUI();

    drawHUD();
}

void BilliardScene::drawAimingUI() const
{
    if (state == BilliardState::BALL_IN_HAND)
    {
        Vector2 mp = GetMousePosition();
        DrawCircleLinesV(mp, BALL_R, Fade(WHITE, 0.6f));
        DrawText("Coloca la bola blanca  (click izquierdo)",
            (int)TABLE_X + 10, (int)(TABLE_Y + TABLE_H + 30), 18, YELLOW);
        return;
    }

    if (state != BilliardState::AIMING || !dragging || !cueBall) return;

    Vector2 mp = GetMousePosition();
    Vector2 diff = Vector2Subtract(cueBall->position, mp);
    float   dist = Vector2Length(diff);
    float   power = fminf(dist / MAX_DRAG, 1.0f);
    Vector2 dir = Vector2Normalize(diff);

    Vector2 cueStart = Vector2Add(mp, Vector2Scale(dir, -20.0f));
    Vector2 cueTip = Vector2Add(cueBall->position,
        Vector2Scale(dir, -(BALL_R + 3.0f)));
    DrawLineEx(cueStart, cueTip, 5.0f, BROWN);
    DrawLineEx(cueStart, cueTip, 2.0f, Fade(BEIGE, 0.6f));

    Vector2 trajEnd = Vector2Add(cueBall->position,
        Vector2Scale(dir, 80.0f + power * 120.0f));
    DrawLineEx(cueBall->position, trajEnd, 1.5f, Fade(WHITE, 0.45f));

    float arrowAng = atan2f(dir.y, dir.x);
    float arrowLen = 10.0f;
    Vector2 a1 = {
        trajEnd.x + cosf(arrowAng + 2.5f) * arrowLen,
        trajEnd.y + sinf(arrowAng + 2.5f) * arrowLen
    };
    Vector2 a2 = {
        trajEnd.x + cosf(arrowAng - 2.5f) * arrowLen,
        trajEnd.y + sinf(arrowAng - 2.5f) * arrowLen
    };
    DrawTriangle(trajEnd, a1, a2, Fade(WHITE, 0.45f));

    const int BAR_X = 10, BAR_Y = 10, BAR_W = 180, BAR_H = 18;
    DrawRectangle(BAR_X - 1, BAR_Y - 1, BAR_W + 2, BAR_H + 2, DARKGRAY);
    DrawRectangle(BAR_X, BAR_Y, BAR_W, BAR_H, Color{ 40, 40, 40, 220 });

    Color barColor = power < 0.4f ? GREEN : power < 0.75f ? YELLOW : RED;
    DrawRectangle(BAR_X, BAR_Y, (int)(BAR_W * power), BAR_H, barColor);
    DrawText("POTENCIA", BAR_X + BAR_W + 8, BAR_Y + 1, 16, WHITE);
}

void BilliardScene::drawHUD() const
{
    int sw = GetScreenWidth();

    DrawText(TextFormat("Bolas embocadas: %d / 15", score),
        10, 40, 20, GOLD);

    if (state == BilliardState::AIMING)
    {
        if (!dragging)
            DrawText("Haz click cerca de la bola blanca y arrastra para apuntar",
                10, GetScreenHeight() - 28, 16, LIGHTGRAY);
        else
            DrawText("Suelta para disparar  |  Click derecho para cancelar",
                10, GetScreenHeight() - 28, 16, LIGHTGRAY);
    }
    else if (state == BilliardState::BALLS_MOVING)
    {
        DrawText("...", sw / 2 - 10, GetScreenHeight() - 28, 18, GRAY);
    }
    else if (state == BilliardState::GAME_OVER)
    {
        int tw = MeasureText("¡GANASTE! Todas las bolas embocadas", 32);
        DrawRectangle(sw / 2 - tw / 2 - 20, GetScreenHeight() / 2 - 30,
            tw + 40, 60, Fade(BLACK, 0.75f));
        DrawText("¡GANASTE! Todas las bolas embocadas",
            sw / 2 - tw / 2, GetScreenHeight() / 2 - 16, 32, GOLD);
    }
}