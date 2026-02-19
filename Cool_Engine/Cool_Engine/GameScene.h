#pragma once
#include "SceneBase.h"
#include "raylib.h"
#include "EventTypes.h"

class GameScene :
    public SceneBase
{
public:

    static GameScene& instance();

    void Load() override;
    void UnLoad() override;
    void UpdateScene() override;
    void Draw() override;
	void update() override;
    void PressButton();
    void OnButtonPress(const ClickButtonEvent& e);
private:
    Rectangle button;
    ClickButtonEvent event;

    void onCollision(const CollisionEvent& event);
};

