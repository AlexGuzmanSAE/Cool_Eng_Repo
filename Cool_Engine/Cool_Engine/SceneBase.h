#pragma once
#include "PhysicsSystem.h"

class SceneBase
{
public:
	std::vector<std::shared_ptr<Entity>> entities;
	
	PhysicsSystem& physics = PhysicsSystem::instance();

	virtual ~SceneBase() = default;
	virtual void Load() = 0;
	virtual void UnLoad() = 0;

	virtual void Update()
	{
		physics.update(GetFrameTime());

		for (auto& entity : entities)
		{
			entity->update();
		}

		entities.erase(std::remove_if(entities.begin(), entities.end(),
			[](const std::shared_ptr<Entity>& entity) { return !entity->isActive(); }),
			entities.end());

	}

	virtual void Draw() = 0;
	virtual void PressButton() = 0;
	bool buttonPressed;
private:


};

