#include "SceneManager.h"

void SceneManager::changeScene(SceneBase* newScene)
{
	if (currentScene)
	{
		currentScene->UnLoad();
	}

	currentScene = newScene;

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
