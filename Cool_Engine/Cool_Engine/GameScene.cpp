#include "GameScene.h"
#include "raylib.h"
#include "EventManager.h"
#include <string>
#include "SceneManager.h"
#include "EventTypes.h"
#include "Log.h"

GameScene& GameScene::instance()
{
	static GameScene instance;
	return instance;
}

void GameScene::onCollision(const CollisionEvent& event)
{
	Log::println("¡Colisión detectada entre " + event.a->getName());
}

void GameScene::Load()
{
	
	button = { 350, 280, 100, 50 };
	
	EventManager::instance().suscribe(this, &GameScene::OnButtonPress);
	EventManager::instance().suscribe<CollisionEvent>(this, &GameScene::onCollision);
}

void GameScene::UnLoad()
{
}

void GameScene::UpdateScene()
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
	DrawText("MenuScene", int(button.x + 10), int(button.y + 10), 20, WHITE);
	SceneBase::Draw();
}

void GameScene::update()
{
}



void GameScene::PressButton()
{
	
}
