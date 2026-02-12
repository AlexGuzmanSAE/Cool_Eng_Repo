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
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 mousePos = GetMousePosition();
		auto ball = physics.make_circle("ball", "ball", 20.0f, true, mousePos);
		addEntity(ball);
	}

}

void GameScene::Draw()
{
	DrawRectangleRec(button, buttonPressed ? RED : DARKBLUE);
	DrawText("MenuScene", button.x + 10, button.y + 10, 20, WHITE);
	SceneBase::Draw();
}



void GameScene::PressButton()
{
	
}
