#pragma once
#include "EventManager.h"

class GameEngine
{
public:
    GameEngine();
    ~GameEngine();
    void Init();
    void Update();
    void Draw();
    void Run();
    void ShutDown();

    static bool shouldQuit;

private:
    int screenWidth = 1280;
    int screenHeight = 720;
    EventManager* eventManagerI = nullptr;
};