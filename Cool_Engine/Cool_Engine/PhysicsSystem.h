#pragma once
#include "box2d/box2d.h"
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
		worldDef.gravity = { 0.0f, 9.8f };
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
		b2World_Step(worldId, dt, 12);
	}

	void process_collisions()
	{

	}

	std::shared_ptr<PBox> create_box(std::string name, std::string tag, 
									 Vector2 size, bool isDynamic, Vector2 position)
	{
		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
		bodyDef.position = { position.x, position.y };
		bodyDef.linearDamping = 0.0f;
		bodyDef.angularDamping = 0.0f;
		b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = 0.001f;
		shapeDef.enableContactEvents = true;

		b2Polygon box = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
		b2CreatePolygonShape(bodyId, &shapeDef, &box);
		auto boxEntity = std::make_shared<PBox>(name, tag, size, isDynamic, bodyId);

		b2Body_SetUserData(bodyId, boxEntity.get());
		
		return boxEntity;
	}
	
	std::shared_ptr<PCircle> make_circle(std::string name, std::string tag, float radius, bool isDynamic, Vector2 position)
	{
		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
		bodyDef.position = { position.x, position.y };
		bodyDef.linearDamping = 0.0f;
		bodyDef.angularDamping = 0.0f;
		b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
		b2Body_SetLinearDamping(bodyId, 0.0f);
		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = 0.01f;
		shapeDef.enableContactEvents = true; 

		b2Circle circle = { {0.0f,  0.0f}, radius };
		auto circleEntity = std::make_shared<PCircle>(name, tag, radius, isDynamic, bodyId);

		b2Body_SetUserData(bodyId, circleEntity.get());
		return circleEntity;
	}

private:
	
	~PhysicsSystem() = default;
	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem& operator=(const PhysicsSystem&) = delete;
};