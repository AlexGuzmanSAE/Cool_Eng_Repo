#include "GameEngine.h"
#include "raylib/raylib.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "MenuScene.h"

GameEngine::GameEngine()
{
    eventManagerI = new EventManager;
}

GameEngine::~GameEngine()
{
    delete eventManagerI;
}

void GameEngine::Init()
{
    InitWindow(screenWidth, screenHeight, "Cool_Engine");
    InitAudioDevice();
    SetTargetFPS(60);

    SceneManager::instance().changeScene(&MenuScene::instance());
}

void GameEngine::Update()
{
    SceneManager::instance().update();
}

void GameEngine::Draw()
{
    BeginDrawing();
    ClearBackground({ 15, 15, 20, 255 });
    SceneManager::instance().draw();
    EndDrawing();
}


bool GameEngine::shouldQuit = false;
void GameEngine::Run()
{
    while (!WindowShouldClose() && !GameEngine::shouldQuit)
    {
        Update();
        Draw();
    }
}

void GameEngine::ShutDown()
{
    CloseAudioDevice();
    CloseWindow();
}