#pragma once
#include "SceneBase.h"
#include "raylib.h"

struct LoadMenuEvent
{
    //cambia escena
};

class GameScene :
    public SceneBase
{
public:

    void Load() override;
    void UnLoad() override;
    void Update() override;
    void Draw() override;
    void PressButton();
    void OnButtonPress(const LoadMenuEvent& e);
private:
    Rectangle button;
    LoadMenuEvent event;
};

