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

    float     pulseTimer = 0.f;
    bool      goToBilliard = false;
    bool      goToQuit = false;
    int       sw = 0, sh = 0;
    bool      goContinue = false;
    bool      hasSave = false;
    Rectangle btnPlay{};
    Rectangle btnSettings{};
    Rectangle btnQuit{};
    Rectangle btnContinue{};

    std::shared_ptr<Sound> sfxClick;
    std::shared_ptr<Music> bgMusic;  
};