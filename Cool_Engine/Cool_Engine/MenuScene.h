#pragma once
#include "SceneBase.h"
#include "raylib.h"
#include <memory>

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

	std::shared_ptr<Music> music;
	std::shared_ptr<Font> font;
};

