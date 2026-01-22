#include "GameScene.h"
#include "raylib.h"


void GameScene::Load()
{
	button = { 350, 280, 100, 50 };
	buttonPressed = false;
}

void GameScene::UnLoad()
{
}

void GameScene::Update()
{
	PressButton();
}

void GameScene::Draw()
{
	DrawRectangleRec(button, buttonPressed ? RED : DARKBLUE);
	DrawText("MenuScene", button.x + 10, button.y + 10, 20, WHITE);

}

void GameScene::PressButton()
{
	if (CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		buttonPressed = true;
	}
}
