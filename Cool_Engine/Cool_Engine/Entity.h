#pragma once
#include <string>
#include "raylib.h"

class Entity
{
public:
	std::string tag;
	std::string name;
	Vector2 position;
	bool active;
	float angle;

	Entity(std::string name, std::string tag)
		: name(name)
		, tag(tag)
		, position({ 0.0f, 0.0f })
		, active(false)
	{
	}

	virtual ~Entity() = default;
	

	virtual void update() = 0;
	virtual void draw() = 0;

	bool isActive() const { return active; }
	void destroy() { active = false; }

	std::string getName() const { return name; }
	std::string getTag() const { return tag; }
};