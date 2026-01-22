#include "MenuScene.h"


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
	PressButton();
}

void MenuScene::Draw()
{
	DrawRectangleRec(button, buttonPressed ? DARKBLUE : RED);
	DrawText("GameScene", button.x + 10, button.y + 10, 20, WHITE);
}

void MenuScene::PressButton()
{
	if (CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		buttonPressed = true;
	}
}
