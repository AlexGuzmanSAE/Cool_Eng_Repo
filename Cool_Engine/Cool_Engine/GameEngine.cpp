#include "GameEngine.h"
#include "raylib.h"
#include "Log.h"
#include "SceneManager.h"

#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

GameEngine::GameEngine()
{
    eventManagerI = new EventManager;
    SceneManager::instance().changeScene("MenuScene");
}

GameEngine::~GameEngine()
{
}

void GameEngine::Init()
{
    InitWindow(screenWidth, screenHeight, "Cool_Engine");
    SetTargetFPS(60);
    
}

void GameEngine::Update()
{
    SceneManager::instance().UpdateScene();
}

void GameEngine::Draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    SceneManager::instance().DrawScene();

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
