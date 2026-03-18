#pragma once
#include "SceneBase.h"
#include "include/raygui.h"

class MenuScene : public SceneBase
{
public:
    static MenuScene& instance()
    {
        static MenuScene inst;
        return inst;
    }

    void Load()        override;
    void UnLoad()      override;
    void Draw()        override;
    void update()      override;
    void PressButton() override {}

private:
    MenuScene() = default;

    bool goToQuit = false;
    float pulseTimer = 0.f;
    bool  goToBilliard = false;

    int sw = 0, sh = 0;
    Rectangle btnPlay{};
    Rectangle btnSettings{};
    Rectangle btnQuit{};
};