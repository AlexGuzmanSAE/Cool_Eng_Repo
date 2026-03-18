#pragma once
#include "SceneBase.h"
#include "PhysicsSystem.h"
#include "PBodies.h"
#include "ResourceManager.h"
#include "SaveSistem.h"
#include "sol/sol.hpp"
#include "raylib/raylib.h"
#include <vector>
#include <memory>
#include <optional>


enum class BilliardState { AIMING, BALLS_MOVING, BALL_IN_HAND, GAME_OVER };

static constexpr Color CUSHION_COLOR = { 22, 100, 28, 255 };
static constexpr Color FELT_COLOR = { 34, 110, 55, 255 };
static constexpr Color TABLE_BORDER = { 90,  48, 12, 255 };

class BilliardScene : public SceneBase
{
public:
    static BilliardScene& instance();

    void Load() override;
    void UnLoad() override;
    void UpdateScene() override;
    void Draw() override;
    void PressButton() override {}
    void update() override {}

    void applyState(const BilliardSave& s);

private:
    BilliardScene() = default;

    float TABLE_X = 0.f;
    float TABLE_Y = 0.f;
    static constexpr float TABLE_W = 800.f;
    static constexpr float TABLE_H = 400.f;
    static constexpr float WALL_T = 20.f;
    static constexpr float BALL_R = 12.f;
    static constexpr float POCKET_R = 18.f;
    static constexpr float MAX_DRAG = 150.f;
    static constexpr float MAX_IMPULSE = 300 * 15000.f;

    std::vector<std::shared_ptr<PCircle>> balls;
    std::vector<std::shared_ptr<PBox>> walls;
    std::shared_ptr<PCircle> cueBall;
    std::vector<Vector2> pockets;
    
    BilliardState state = BilliardState::AIMING;
    int score = 0;
    bool dragging = false;
    Vector2 dragPos = {};

    bool paused = false;
    Rectangle btnResume = {};
    Rectangle btnSave = {};
    Rectangle btnMenu = {};

    std::shared_ptr<Sound> sfxShot;
    std::shared_ptr<Sound> sfxPocket;
    std::shared_ptr<Music> bgMusic;

    std::unique_ptr<sol::state> lua;
    sol::safe_function luaUpdate;
    sol::safe_function luaDraw;

    void buildTable();
    void rackBalls();
    void placeCueBall(Vector2 pos);
    bool allStopped() const;
    void checkPockets();
    void bindRaylib();
    void drawAimingUI() const;
    void drawHUD()      const;
    void drawPauseMenu();
    BilliardSave captureState() const;

    std::optional<BilliardSave> pendingSave;
};