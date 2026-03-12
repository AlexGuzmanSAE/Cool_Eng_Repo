#pragma once
#include "SceneBase.h"
#include "PhysicsSystem.h"
#include "PBodies.h"
#include <memory>
#include <vector>
#include <string>

enum class BilliardState {
    AIMING,
    BALLS_MOVING,
    BALL_IN_HAND,
    GAME_OVER
};

class BilliardScene : public SceneBase {
public:
    static BilliardScene& instance();

    void Load()        override;
    void UnLoad()      override;
    void UpdateScene() override;
    void Draw()        override;

private:
    BilliardScene() = default;

    void update() override {} 
    void PressButton() override {}

    PhysicsSystem& physics = PhysicsSystem::instance();

    static constexpr float TABLE_X = 100.0f;
    static constexpr float TABLE_Y = 100.0f;
    static constexpr float TABLE_W = 600.0f;
    static constexpr float TABLE_H = 340.0f;
    static constexpr float WALL_T = 20.0f;

    static constexpr float BALL_R = 11.0f;
    static constexpr float POCKET_R = 16.0f;

    static constexpr float MAX_DRAG = 140.0f;
    static constexpr float MAX_IMPULSE = 22.0f;

    std::shared_ptr<PCircle>              cueBall;
    std::vector<std::shared_ptr<PCircle>> balls;
    std::vector<std::shared_ptr<PBox>>    walls;
    std::vector<Vector2>                  pockets;

    BilliardState state = BilliardState::AIMING;
    int           score = 0;
    bool          dragging = false;
    Vector2       dragPos = {};

    void  buildTable();
    void  rackBalls();
    void  placeCueBall(Vector2 pos);
    bool  allStopped()   const;
    void  checkPockets();
    void  drawAimingUI() const;
    void  drawHUD()      const;

    static constexpr Color FELT_COLOR = { 22,  99,  40, 255 };
    static constexpr Color CUSHION_COLOR = { 80,  50,  20, 255 };
    static constexpr Color TABLE_BORDER = { 50,  30,  10, 255 };
};