#include "GameEngine.h"

int main(void)
{
    GameEngine gameEngine;
    gameEngine.Init();
    gameEngine.Run();
    gameEngine.Draw();
    gameEngine.ShutDown();

    return 0;
}
