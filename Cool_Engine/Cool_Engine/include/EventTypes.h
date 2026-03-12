#pragma once

#include <string>
#include "Entity.h"

struct ClickButtonEvent {
    std::string buttonName = "my button";
};

struct CollisionEvent {
    Entity* a;
    Entity* b;
    float force;
};