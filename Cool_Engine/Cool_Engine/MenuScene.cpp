#include "MenuScene.h"
#include "EventManager.h"
#include <string>
#include "SceneManager.h"
#include "ResourceManager.h"

void MenuScene::OnButtonPress(const LoadGameEvent& e)
{
	SceneManager::instance().changeScene("GameScene");
}

void MenuScene::Load()
{
	button = { 350, 280, 100, 50 };
	EventManager::instance().suscribe(this, &MenuScene::OnButtonPress);

	music = ResourceManager::instance().load_music("cat_walk.ogg");
	PlayMusicStream(*music);
}

void MenuScene::UnLoad()
{
}

void MenuScene::Update()
{
	if (CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		EventManager::instance().emit(event);
	}
	UpdateMusicStream(*music);
}

void MenuScene::Draw()
{
	DrawRectangleRec(button, buttonPressed ? DARKBLUE : RED);
	DrawText("GameScene", button.x + 10, button.y + 10, 20, WHITE);
	SceneBase::Draw();
}

void MenuScene::PressButton()
{
}



