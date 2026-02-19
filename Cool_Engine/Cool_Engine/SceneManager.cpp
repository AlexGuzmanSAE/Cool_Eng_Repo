#include "SceneManager.h"

SceneManager& SceneManager::instance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::update()
{
    if (currentScene)
        currentScene->UpdateScene();
}

void SceneManager::draw()
{
    if (currentScene)
        currentScene->draw_scene();
}

void SceneManager::changeScene(SceneBase* newScene)
{
    if (currentScene)
        currentScene->UnLoad();

    currentScene = newScene;

    if (currentScene)
        currentScene->Load();
}
