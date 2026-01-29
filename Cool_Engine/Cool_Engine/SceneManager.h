#pragma once
#include "SceneBase.h"
#include <string>
#include "GameScene.h"
#include "MenuScene.h"

class SceneManager
{
public:
	static SceneManager& instance()
	{
		static SceneManager sM;
		return sM;
	}
	
	void changeScene(std::string sceneName);
	void UpdateScene();
	void DrawScene();
	SceneBase* GetCurrentScene();

	SceneBase* gameScene;
	SceneBase* menuScene;
	SceneBase* currentScene;
private:
	SceneManager();
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;


	std::string gameSceneStr = "GameScene";
	std::string menuSceneStr = "MenuScene";
	std::string currentSceneStr;
};