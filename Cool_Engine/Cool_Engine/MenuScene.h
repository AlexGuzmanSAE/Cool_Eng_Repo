#pragma once
#include "SceneBase.h"
#include "raylib.h"

class MenuScene :
    public SceneBase
{
public:

    void Load() override;
    void UnLoad() override;
    void Update() override;
    void Draw() override;

    

private:
    Rectangle button;
};

