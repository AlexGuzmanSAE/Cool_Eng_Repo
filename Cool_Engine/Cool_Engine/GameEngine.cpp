#include "GameEngine.h"
#include "raylib.h"
#include "Log.h"

GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::Init()
{
    Log::println("INIT");
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);
}

void GameEngine::Update()
{
    Log::println("UPDATE");
    DrawRectangle(100, 100, 20, 20, RED);
}

void GameEngine::Draw()
{
    Log::println("DRAW");
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}

void GameEngine::Run()
{
    Log::println("RUN");
    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }
    
}

void GameEngine::ShutDown()
{
    Log::println("SHUTDOWN");
    CloseWindow();
}
