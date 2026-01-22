#include "MenuScene.h"
#include "raylib.h"

void MenuScene::Load()
{
	button = { 350, 280, 100, 50 };
	buttonPressed = false;
}

void MenuScene::UnLoad()
{
}

void MenuScene::Update()
{

	if (CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		buttonPressed = true;
	}
}

void MenuScene::Draw()
{
	DrawRectangleRec(button, buttonPressed ? RED : DARKBLUE);
	DrawText("GameScene", button.x + 10, button.y + 10, 20, WHITE);
}