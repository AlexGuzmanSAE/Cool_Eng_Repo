#include "GameScene.h"
#include "raylib/raylib.h"
#include "EventManager.h"
#include <string>
#include "SceneManager.h"
#include "EventTypes.h"
#include "Log.h"
#include <iostream>
//#include <fstream>
//#include "nlohmann/json.hpp"
//
//using json = nlohmann::json;

GameScene& GameScene::instance()
{ 
	static GameScene instance;
	return instance;
}

void GameScene::onCollision(const CollisionEvent& event)
{
	Log::println("¡Colisión detectada entre " + event.a->getName());
}

void GameScene::bindRaylib()
{
	lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
	sol::table rl = lua.create_named_table("raylib");

	rl.set_function("draw_circle", [](float x, float y, float r, sol::optional<Color> c)
		{
			Color color = c.value_or(GREEN);
			Vector2 center = { x, y };
			DrawCircleV(center, r, color);
		});
	rl.set_function("mouse_pressed", &IsMouseButtonPressed);
	rl.set_function("mouse_x", &GetMouseX);
	rl.set_function("mouse_y", &GetMouseY);
	//rl.set_function("to_world", [&](float sx, float sy)
	//    {
	//        /*Vector2 world = GetScreenToWorld2D({sx, sy}, cam);
	//        return std::make_tuple(world.x, world.y);*/
	//    });
	rl.set_function("print", [](std::string message)
		{
			std::cout << "[LUA]: " << message << std::endl;
		});
	rl["green"] = GREEN;
	auto result = lua.script_file("assets/scripts/game_scene.lua");
	if (result.valid())
	{
		luaUpdate = lua["update"];
		luaDraw = lua["draw"];
		std::cout << "Script cargado y funciones vinculadas." << std::endl;
	}
	else
	{
		sol::error err = result;
		std::cerr << "Error al cargar el script: " << err.what() << std::endl;
	}
}

void GameScene::Load()
{
	bindRaylib();
	button = { 350, 280, 100, 50 };
	
	//EventManager::instance().suscribe(this, &GameScene::OnButtonPress);
	EventManager::instance().suscribe<CollisionEvent>(this, &GameScene::onCollision);

	//std::ifstream file("assets/json/gameScene.json");
	//if (file.is_open())
	//{
	//	
	// json j;
	//	file >> j;
	//	Log::println("JSON loaded successfully!");
	//	Log::println("Button position: (" + std::to_string(j["button"]["x"].get<float>()) + ", " + std::to_string(j["button"]["y"].get<float>()) + ")");
	//}
	//else
	//{
	//	Log::println("Failed to open JSON file!");
	//}
}

void GameScene::UnLoad()
{
}

void GameScene::UpdateScene()
{
	if (luaUpdate)

	{
	luaUpdate();

	}


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
	if (luaDraw)

	{
		luaDraw();

	}
	DrawRectangleRec(button, buttonPressed ? RED : DARKBLUE);
	DrawText("MenuScene", int(button.x + 10), int(button.y + 10), 20, WHITE);
	SceneBase::draw_scene();
}

void GameScene::update()
{
}


void GameScene::PressButton()
{
	
}
