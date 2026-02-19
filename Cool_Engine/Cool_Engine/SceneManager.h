#pragma once
#include "SceneBase.h"

class SceneManager
{
public:
    static SceneManager& instance();

    void update();
    void draw();
    void changeScene(SceneBase* newScene);

private:
    SceneManager() = default;
    SceneBase* currentScene = nullptr;
};
