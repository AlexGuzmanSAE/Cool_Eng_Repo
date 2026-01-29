#include "SceneManager.h"

SceneManager::SceneManager()
{
	gameScene = new GameScene;
	menuScene = new MenuScene;
	currentScene = nullptr;
}
void SceneManager::changeScene(std::string sceneName)
{
	if (currentScene)
	{
		currentScene->UnLoad();
	}

	currentSceneStr = sceneName;

	if (gameSceneStr == sceneName)
	{
		currentScene = gameScene;
	}
	else if (sceneName == menuSceneStr)
	{
		currentScene = menuScene;
	}

	if (currentScene)
	{
		currentScene->Load();
	}
}

void SceneManager::UpdateScene()
{
	currentScene->Update();
}

void SceneManager::DrawScene()
{
	currentScene->Draw();
}

SceneBase* SceneManager::GetCurrentScene()
{
	return currentScene;
}

