#include "GameScene.h"
#include "raylib.h"
#include "EventManager.h"
#include <string>
#include "SceneManager.h"

void GameScene::OnButtonPress(const LoadMenuEvent& e)
{
	SceneManager::instance().changeScene("MenuScene");
}

void GameScene::Load()
{
	button = { 350, 280, 100, 50 };
	
	EventManager::instance().suscribe(this, &GameScene::OnButtonPress);

}

void GameScene::UnLoad()
{
}

void GameScene::Update()
{
	if (CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		EventManager::instance().emit(event);
	}
}

void GameScene::Draw()
{
	DrawRectangleRec(button, buttonPressed ? RED : DARKBLUE);
	DrawText("MenuScene", button.x + 10, button.y + 10, 20, WHITE);

}



void GameScene::PressButton()
{
	
}
