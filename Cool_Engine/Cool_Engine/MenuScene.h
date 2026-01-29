#pragma once
#include "SceneBase.h"
#include "raylib.h"

struct LoadGameEvent
{
    //cambia escena
};

class MenuScene :
    public SceneBase
{
public:

    void Load() override;
    void UnLoad() override;
    void Update() override;
    void Draw() override;
    void PressButton() override;
    void OnButtonPress(const LoadGameEvent& e);

private:
    Rectangle button;
    LoadGameEvent event;
};

