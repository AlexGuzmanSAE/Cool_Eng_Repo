#pragma once
#include "SceneBase.h"

class SceneManager
{
public:
	static SceneManager& instance()
	{
		static SceneManager sM;
		return sM;
	}

	void changeScene(SceneBase* newScene);
	void UpdateScene();
	void DrawScene();
	SceneBase* GetCurrentScene();
private:
	SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	SceneBase* currentScene;
};