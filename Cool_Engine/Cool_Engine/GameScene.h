#pragma once
#include "SceneBase.h"
#include "raylib.h"

class GameScene :
    public SceneBase
{
public:

    void Load() override;
    void UnLoad() override;
    void Update() override;
    void Draw() override;
    void PressButton() override;
private:
    Rectangle button;
};

