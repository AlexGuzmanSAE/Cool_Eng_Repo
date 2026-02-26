#include "GameEngine.h"
#include "raylib.h"
#include "Log.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "box2d/base.h"
#include "GameScene.h"

//#define RAYGUI_IMPLEMENTATION
//#include "include/raygui.h"

GameEngine::GameEngine()
{
    eventManagerI = new EventManager;
}

GameEngine::~GameEngine()
{
}

void GameEngine::Init()
{
    InitWindow(screenWidth, screenHeight, "Cool_Engine");
	InitAudioDevice();
    SceneManager::instance().changeScene(&GameScene::instance());
    SetTargetFPS(60);
}

void GameEngine::Update()
{
    SceneManager::instance().update();
}

void GameEngine::Draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

	SceneManager::instance().draw();

    EndDrawing();
}

void GameEngine::Run()
{
    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }
    
}

void GameEngine::ShutDown()
{
    CloseWindow();
}
