#pragma once

#include "EventManager.h"
#include "ResourceManager.h"
#include "PhysicsSystem.h"
#include <vector>
#include <algorithm>


class SceneBase
{
public:
	std::vector<std::shared_ptr<Entity>> entities;
		
	//Camera2D camera;
	EventManager& eventManager = EventManager::instance();
	ResourceManager& resourceManager = ResourceManager::instance();
	PhysicsSystem& physics = PhysicsSystem::instance();


	//SceneBase()
	//	: buttonPressed(false)
	//{
	//	//camera.offset = { 0.0f, 0.0f};
	//	//camera.zoom = 20.0f;
	//}

	virtual void UpdateScene()
	{
		update();
		physics.update(GetFrameTime());

		for (auto& entity : entities)
		{
			entity->update();
		}

		entities.erase(std::remove_if(entities.begin(), entities.end(),
			[](const std::shared_ptr<Entity>& entity) { return !entity->isActive(); }),
			entities.end());

	}

	void draw_scene()
	{
		//BeginMode2D(camera);
		for (auto& entity : entities)
		{
			entity->draw();
		}
		Draw();
		//EndMode2D();
	}


	virtual ~SceneBase() = default;
	virtual void Load() = 0;
	virtual void UnLoad() = 0;
	virtual void Draw() = 0;
	virtual void PressButton() = 0;
	virtual void update() = 0;

	bool buttonPressed;

	void clearScene()
	{
		entities.clear();
	}

	void addEntity(std::shared_ptr<Entity> entity)
	{
		entities.push_back(entity);
	}

};

