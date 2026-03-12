#pragma once
#include "box2d/box2d.h"
#include "raylib.h"
#include "raymath.h"
#include "PhysicsEntity.h"

class PBox : public PhysicsEntity
{
public:
	Vector2 size;

	PBox(std::string name, std::string tag, Vector2 size, bool b, b2BodyId id)
		: PhysicsEntity(name, tag, id, b)
		, size(size)
	{
	}

	void draw() override
	{
		Vector2 h = { size.x / 2.0f, size.y / 2.0f };

		b2Vec2 v1 = b2Body_GetWorldPoint(bodyId, { -h.x, -h.y });
		b2Vec2 v2 = b2Body_GetWorldPoint(bodyId, { h.x, -h.y });
		b2Vec2 v3 = b2Body_GetWorldPoint(bodyId, { -h.x, h.y });
		b2Vec2 v4 = b2Body_GetWorldPoint(bodyId, { h.x, h.y });

		DrawRectanglePro({ position.x, position.y, size.x, size.y }, h,
			angle, Fade(color, 0.5f));

		DrawLineV({ v1.x, v1.y }, { v2.x, v2.y }, color);
		DrawLineV({ v2.x, v2.y }, { v3.x, v3.y }, color);
		DrawLineV({ v3.x, v3.y }, { v4.x, v4.y }, color);
		DrawLineV({ v4.x, v4.y }, { v1.x, v1.y }, color);

	}
	void setBodyId(const b2BodyId& id) { bodyId = id; }
	b2BodyId getBodyId() const { return bodyId; }
};

class PCircle : public PhysicsEntity
{
	float radius;
		
public:
	PCircle(std::string name, std::string tag, float radius, bool b, b2BodyId id)
		: PhysicsEntity(name, tag, id, b)
		, radius(radius)
	{
	}

	void draw() override
	{
		DrawCircleLinesV(position, radius, color);
		DrawCircleV(position, radius, Fade(color, 0.5f));

		Vector2 dir = { cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) };
		Vector2 scaleDir = Vector2Scale(dir, radius);
		Vector2 endPoint = Vector2Add(position, scaleDir);
		DrawLineV(position, endPoint, color);
	}

	b2BodyId getBodyId() const { return bodyId; }
};