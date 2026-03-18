#include "BilliardScene.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "AudioHelper.h"
#include "box2d/box2d.h"
#include <cmath>
#include <string>
#include "SaveSistem.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include "include/raygui.h"

BilliardScene& BilliardScene::instance()
{
    static BilliardScene inst;
    return inst;
}

void BilliardScene::Load()
{
    TABLE_X = (GetScreenWidth() - TABLE_W) * 0.5f;
    TABLE_Y = (GetScreenHeight() - TABLE_H) * 0.5f;

    luaUpdate = sol::safe_function{};
    luaDraw = sol::safe_function{};
    lua = std::make_unique<sol::state>();

    physics.reset();
    b2World_SetGravity(physics.worldId, { 0.0f, 0.0f });

    buildTable();

    if (pendingSave.has_value())
    {
        BilliardSave save = pendingSave.value();
        pendingSave.reset();

        score = save.score;

        for (auto& bs : save.balls)
        {
            auto ball = physics.make_billiard_circle(
                "ball", "ball", BALL_R, true, { bs.x, bs.y });
            ball->color = { (unsigned char)bs.r, (unsigned char)bs.g,
                            (unsigned char)bs.b, (unsigned char)bs.a };
            balls.push_back(ball);
        }

        if (save.hasCueBall)
            placeCueBall({ save.cueBall.x, save.cueBall.y });
    }
    else
    {
        score = 0;
        rackBalls();
        placeCueBall({ TABLE_X + TABLE_W * 0.25f, TABLE_Y + TABLE_H * 0.5f });
    }

    state = BilliardState::AIMING;
    paused = false;

    sfxShot = ResourceManager::instance().load_sounds("golpeBillar.wav");
    sfxPocket = ResourceManager::instance().load_sounds("ballIn.wav");
    bgMusic = ResourceManager::instance().load_music("game.ogg");

    if (bgMusic)
    {
        Audio_SetMusicVolume(*bgMusic, 0.4f);
        PlayMusicStream(*bgMusic);
    }

    bindRaylib();
}

void BilliardScene::UnLoad()
{
    if (bgMusic) Audio_StopMusic(*bgMusic);

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

    auto bot = physics.create_billiard_box("wall_bot", "wall",
        { TABLE_W + WALL_T * 2.0f, WALL_T }, false,
        { cx, TABLE_Y + TABLE_H + WALL_T * 0.5f });
    bot->color = CUSHION_COLOR;
    walls.push_back(bot);

    auto lft = physics.create_billiard_box("wall_left", "wall",
        { WALL_T, TABLE_H }, false,
        { TABLE_X - WALL_T * 0.5f, cy });
    lft->color = CUSHION_COLOR;
    walls.push_back(lft);

    auto rgt = physics.create_billiard_box("wall_right", "wall",
        { WALL_T, TABLE_H }, false,
        { TABLE_X + TABLE_W + WALL_T * 0.5f, cy });
    rgt->color = CUSHION_COLOR;
    walls.push_back(rgt);

    pockets = {
        { TABLE_X,                   TABLE_Y           },
        { TABLE_X + TABLE_W * 0.5f, TABLE_Y           },
        { TABLE_X + TABLE_W,        TABLE_Y           },
        { TABLE_X,                   TABLE_Y + TABLE_H },
        { TABLE_X + TABLE_W * 0.5f, TABLE_Y + TABLE_H },
        { TABLE_X + TABLE_W,        TABLE_Y + TABLE_H },
    };
}

void BilliardScene::rackBalls()
{
    const Color COLORS[15] = {
        YELLOW, BLUE, RED, PURPLE, ORANGE, DARKGREEN, MAROON,
        BLACK,
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
    constexpr float THRESHOLD = 0.1f;

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
    lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);

    lua->new_usertype<Color>("Color",
        "r", &Color::r, "g", &Color::g,
        "b", &Color::b, "a", &Color::a);

    sol::table rl = lua->create_named_table("rl");

    rl.set_function("draw_circle",
        [](float x, float y, float r, Color c) { DrawCircleV({ x,y }, r, c); });
    rl.set_function("draw_circle_lines",
        [](float x, float y, float r, Color c) { DrawCircleLinesV({ x,y }, r, c); });
    rl.set_function("draw_rectangle",
        [](float x, float y, float w, float h, Color c) {
            DrawRectangle((int)x, (int)y, (int)w, (int)h, c); });
    rl.set_function("draw_line",
        [](float x1, float y1, float x2, float y2, Color c) {
            DrawLine((int)x1, (int)y1, (int)x2, (int)y2, c); });
    rl.set_function("draw_line_ex",
        [](float x1, float y1, float x2, float y2, float thick, Color c) {
            DrawLineEx({ x1,y1 }, { x2,y2 }, thick, c); });
    rl.set_function("draw_triangle",
        [](float x1, float y1, float x2, float y2, float x3, float y3, Color c) {
            DrawTriangle({ x1,y1 }, { x2,y2 }, { x3,y3 }, c); });
    rl.set_function("draw_text",
        [](const std::string& s, int x, int y, int sz, Color c) {
            DrawText(s.c_str(), x, y, sz, c); });
    rl.set_function("measure_text",
        [](const std::string& s, int sz) { return MeasureText(s.c_str(), sz); });
    rl.set_function("fade",
        [](Color c, float a) { return Fade(c, a); });
    rl.set_function("make_color",
        [](int r, int g, int b, int a) -> Color {
            return { (unsigned char)r, (unsigned char)g,
                     (unsigned char)b, (unsigned char)a }; });

    rl.set_function("mouse_pressed", &IsMouseButtonPressed);
    rl.set_function("mouse_down", &IsMouseButtonDown);
    rl.set_function("mouse_released", &IsMouseButtonReleased);
    rl.set_function("mouse_x", &GetMouseX);
    rl.set_function("mouse_y", &GetMouseY);
    rl.set_function("screen_width", &GetScreenWidth);
    rl.set_function("screen_height", &GetScreenHeight);

    rl["WHITE"] = WHITE;     rl["BLACK"] = BLACK;
    rl["RED"] = RED;       rl["GREEN"] = GREEN;
    rl["BLUE"] = BLUE;      rl["YELLOW"] = YELLOW;
    rl["ORANGE"] = ORANGE;    rl["PURPLE"] = PURPLE;
    rl["GRAY"] = GRAY;      rl["DARKGRAY"] = DARKGRAY;
    rl["LIGHTGRAY"] = LIGHTGRAY; rl["GOLD"] = GOLD;
    rl["BROWN"] = BROWN;     rl["BEIGE"] = BEIGE;
    rl["MAROON"] = MAROON;    rl["DARKGREEN"] = DARKGREEN;

    rl["MOUSE_LEFT"] = MOUSE_BUTTON_LEFT;
    rl["MOUSE_RIGHT"] = MOUSE_BUTTON_RIGHT;

    rl.set_function("print",
        [](const std::string& s) { std::cout << "[LUA] " << s << "\n"; });

    sol::table game = lua->create_named_table("game");

    game["TABLE_X"] = TABLE_X;
    game["TABLE_Y"] = TABLE_Y;
    game["TABLE_W"] = TABLE_W;
    game["TABLE_H"] = TABLE_H;
    game["WALL_T"] = WALL_T;
    game["BALL_R"] = BALL_R;
    game["POCKET_R"] = POCKET_R;
    game["MAX_DRAG"] = MAX_DRAG;
    game["MAX_IMPULSE"] = MAX_IMPULSE;

    game.set_function("get_state", [this]() -> std::string {
        switch (state) {
        case BilliardState::AIMING:       return "AIMING";
        case BilliardState::BALLS_MOVING: return "BALLS_MOVING";
        case BilliardState::BALL_IN_HAND: return "BALL_IN_HAND";
        case BilliardState::GAME_OVER:    return "GAME_OVER";
        default:                          return "UNKNOWN";
        }
        });

    game.set_function("set_state", [this](const std::string& s) {
        if (s == "AIMING")       state = BilliardState::AIMING;
        else if (s == "BALLS_MOVING") state = BilliardState::BALLS_MOVING;
        else if (s == "BALL_IN_HAND") state = BilliardState::BALL_IN_HAND;
        else if (s == "GAME_OVER")    state = BilliardState::GAME_OVER;
        });

    game.set_function("cue_ball_pos", [this]() -> std::tuple<float, float, bool> {
        if (!cueBall) return { 0.f, 0.f, false };
        return { cueBall->position.x, cueBall->position.y, true };
        });

    game.set_function("get_balls", [this]() {
        sol::table t = lua->create_table();
        for (int i = 0; i < (int)balls.size(); i++) {
            sol::table b = lua->create_table();
            b["x"] = balls[i]->position.x;
            b["y"] = balls[i]->position.y;
            b["color"] = balls[i]->color;
            t[i + 1] = b;
        }
        return t;
        });

    game.set_function("get_pockets", [this]() {
        sol::table t = lua->create_table();
        for (int i = 0; i < (int)pockets.size(); i++) {
            sol::table p = lua->create_table();
            p["x"] = pockets[i].x;
            p["y"] = pockets[i].y;
            t[i + 1] = p;
        }
        return t;
        });

    game.set_function("ball_count", [this]() { return (int)balls.size(); });
    game.set_function("all_stopped", [this]() { return allStopped(); });

    game.set_function("check_pockets", [this]() -> std::tuple<int, bool> {
        int  pocketed = 0;
        bool cuePocketed = false;

        for (auto it = balls.begin(); it != balls.end(); ) {
            bool hit = false;
            for (auto& p : pockets) {
                if (Vector2Distance((*it)->position, p) < POCKET_R + BALL_R * 0.5f) {
                    b2DestroyBody((*it)->getBodyId());
                    it = balls.erase(it);
                    pocketed++;
                    hit = true;
                    break;
                }
            }
            if (!hit) ++it;
        }
        if (cueBall) {
            for (auto& p : pockets) {
                if (Vector2Distance(cueBall->position, p) < POCKET_R + BALL_R * 0.5f) {
                    b2DestroyBody(cueBall->getBodyId());
                    cueBall = nullptr;
                    cuePocketed = true;
                    break;
                }
            }
        }
        return { pocketed, cuePocketed };
        });

    game.set_function("apply_shot", [this](float vx, float vy) {
        if (cueBall)
            b2Body_ApplyLinearImpulseToCenter(cueBall->getBodyId(), { vx, vy }, true);
        });

    game.set_function("place_cue_ball", [this](float x, float y) {
        placeCueBall({ x, y });
        });

    game.set_function("play_sfx_shot", [this]() { if (sfxShot)   Audio_PlaySound(*sfxShot);   });
    game.set_function("play_sfx_pocket", [this]() { if (sfxPocket) Audio_PlaySound(*sfxPocket); });
    game.set_function("update_music", [this]() { if (bgMusic)   Audio_UpdateMusic(*bgMusic); });

    auto result = lua->script_file("assets/scripts/game_scene.lua");
    if (result.valid()) {
        luaUpdate = (*lua)["update"];
        luaDraw = (*lua)["draw"];
        std::cout << "Script loaded OK\n";
    }
    else {
        sol::error err = result;
        std::cerr << "Lua error: " << err.what() << "\n";
    }
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
    Vector2 cueTip = Vector2Add(cueBall->position, Vector2Scale(dir, -(BALL_R + 3.0f)));
    DrawLineEx(cueStart, cueTip, 5.0f, BROWN);
    DrawLineEx(cueStart, cueTip, 2.0f, Fade(BEIGE, 0.6f));

    Vector2 trajEnd = Vector2Add(cueBall->position,
        Vector2Scale(dir, 80.0f + power * 120.0f));
    DrawLineEx(cueBall->position, trajEnd, 1.5f, Fade(WHITE, 0.45f));

    float arrowAng = atan2f(dir.y, dir.x);
    float arrowLen = 10.0f;
    Vector2 a1 = { trajEnd.x + cosf(arrowAng + 2.5f) * arrowLen,
                   trajEnd.y + sinf(arrowAng + 2.5f) * arrowLen };
    Vector2 a2 = { trajEnd.x + cosf(arrowAng - 2.5f) * arrowLen,
                   trajEnd.y + sinf(arrowAng - 2.5f) * arrowLen };
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
    DrawText(TextFormat("Bolas embocadas: %d / 15", score), 10, 40, 20, GOLD);

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

BilliardSave BilliardScene::captureState() const
{
    BilliardSave s;
    s.score = score;
    s.hasCueBall = (cueBall != nullptr);

    if (cueBall)
        s.cueBall = { cueBall->position.x, cueBall->position.y,
                      cueBall->color.r,    cueBall->color.g,
                      cueBall->color.b,    cueBall->color.a };

    for (auto& b : balls)
        s.balls.push_back({ b->position.x, b->position.y,
                            b->color.r,    b->color.g,
                            b->color.b,    b->color.a });
    return s;
}

void BilliardScene::applyState(const BilliardSave& s)
{
    pendingSave = s;
}

void BilliardScene::UpdateScene()
{
    if (IsKeyPressed(KEY_P))
        paused = !paused;

    if (paused) return;

    if (luaUpdate) luaUpdate();

    float dt = GetFrameTime();
    physics.update(dt);
    for (auto& w : walls) w->syncFromBody();
    for (auto& b : balls) b->syncFromBody();
    if (cueBall) cueBall->syncFromBody();
}

void BilliardScene::drawPauseMenu()
{
    int sw = GetScreenWidth(), sh = GetScreenHeight();

    DrawRectangle(0, 0, sw, sh, { 0, 0, 0, 140 });

    float pw = 320, ph = 220;
    float px = (sw - pw) * 0.5f, py = (sh - ph) * 0.5f;
    DrawRectangleRounded({ px, py, pw, ph }, 0.1f, 8, { 20, 20, 30, 240 });

    float btnW = 240, btnH = 48, gap = 14;
    float bx = px + (pw - btnW) * 0.5f;
    float by = py + 30;
    btnResume = { bx, by, btnW, btnH };
    btnSave = { bx, by + btnH + gap, btnW, btnH };
    btnMenu = { bx, by + (btnH + gap) * 2, btnW, btnH };

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x1A6B30FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x28A045FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x0F4A20FF);
    if (GuiButton(btnResume, "#131# Continuar"))
        paused = false;

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x2C3E50FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x3D5166FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x1A252FFF);
    if (GuiButton(btnSave, "#2# Guardar partida"))
        SaveSistem::instance().saveBilliard(captureState());

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x7B1010FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xA01515FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x4A0A0AFF);
    if (GuiButton(btnMenu, "#113# Menu principal"))
        SceneManager::instance().changeScene(&MenuScene::instance());
}

void BilliardScene::Draw()
{
    if (luaDraw) luaDraw();
    if (paused)  drawPauseMenu();
}