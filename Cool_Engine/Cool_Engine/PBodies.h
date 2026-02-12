#pragma once
#include "Entity.h"
#include "box2d/box2d.h"
#include "raylib.h"
#include "raymath.h"

class PBox : public Entity
{
public:
	b2BodyId bodyId;
	Vector2 size;
	Color color;

	PBox(std::string name, std::string tag, Vector2 size, bool b, b2BodyId id)
		: Entity(name, tag)
		, bodyId(id)
		, size(size)
	{
		b2Vec2 b2pos = b2Body_GetPosition(bodyId);
		this->position = { b2pos.x, b2pos.y };
		b2Rot b2rot = b2Body_GetRotation(bodyId);
		this->angle = b2Rot_GetAngle(b2rot) * RAD2DEG;
		color = b ? GREEN : BLUE;
	}
	~PBox() { if (b2Body_IsValid(bodyId)) b2DestroyBody(bodyId); }
	void update() override
	{
		b2Vec2 b2pos = b2Body_GetPosition(bodyId);
		position = { b2pos.x, b2pos.y };
		angle = b2Rot_GetAngle(b2Body_GetRotation(bodyId)) * RAD2DEG;
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

class PCircle : public Entity
	{
		b2BodyId bodyId;
		float radius;
		Color color;

		PCircle(std::string name, std::string tag, float radius, bool b, b2BodyId id)
			: Entity(name, tag)
			, bodyId(id)
			, radius(radius)
		{
			b2Vec2 b2pos = b2Body_GetPosition(bodyId);
			this->position = { b2pos.x, b2pos.y };

			b2Rot b2rot = b2Body_GetRotation(bodyId);
			this->angle = b2Rot_GetAngle(b2rot) * RAD2DEG;
			this->color = b ? BLUE : RED;
			color = b ? ORANGE : BLUE;
		}

		~PCircle() { if (b2Body_IsValid(bodyId)) b2DestroyBody(bodyId); }

		void update() override
		{
			b2Vec2 b2pos = b2Body_GetPosition(bodyId);
			position = { b2pos.x, b2pos.y };
			angle = b2Rot_GetAngle(b2Body_GetRotation(bodyId)) * RAD2DEG;
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


	void setBodyId(const b2BodyId& id) { bodyId = id; }
	b2BodyId getBodyId() const { return bodyId; }
};