#pragma once
#include "PBodies.h"
#include <memory>

class PhysicsSystem
{
public:
	b2WorldId worldId;

	PhysicsSystem()
	{
		init_world();
	}

	void init_world()
	{
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = { 0.0f, 980.0f };
		worldId = b2CreateWorld(&worldDef);
	}

	static PhysicsSystem& instance()
	{
		static PhysicsSystem instance;
		return instance;
	}

	void reset()
	{
		if (b2World_IsValid(worldId))
		{
			b2DestroyWorld(worldId);
		}
		init_world();
	}

	void update(float dt)
	{
		b2World_Step(worldId, dt, 4);
	}

	std::shared_ptr<PBox> create_box(std::string name, std::string tag, 
									 Vector2 size, bool isDynamic, Vector2 position)
	{
		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
		bodyDef.position = { position.x, position.y };
		b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
		b2ShapeDef shapeDef = b2DefaultShapeDef();
		b2Polygon box = b2MakeBox(size.x / 2.0f, size.y / 2.0f);

		return std::make_shared<PBox>(name, tag, size, isDynamic, bodyId);
	}
	
	std::shared_ptr<PCircle> make_circle(std::string name, std::string tag, float radius, bool isDynamic, Vector2 position)
	{
		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
		bodyDef.position = { position.x, position.y };
		b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
		b2ShapeDef shapeDef = b2DefaultShapeDef();
		b2Circle circle = { {0.0f, 0.0f}, radius };

		return std::make_shared<PCircle>(name, tag, radius, isDynamic, bodyId);
	}

private:
	
	~PhysicsSystem() = default;
	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem& operator=(const PhysicsSystem&) = delete;
};