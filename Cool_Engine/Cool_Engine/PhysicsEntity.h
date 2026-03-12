#pragma once
#include "Entity.h"
#include "box2d/box2d.h"

class PhysicsEntity;

struct Collision
{
	PhysicsEntity* self;
    PhysicsEntity* other;
    float force;
};

class PhysicsEntity : public Entity {
public:
    b2BodyId bodyId;
    Color color;

public:
    PhysicsEntity(std::string n, std::string t, b2BodyId id, bool isDynamic)
        : Entity(n, t), bodyId(id) {
        this->color = isDynamic ? RED : GREEN;
        updatePhysics();
    }

    virtual ~PhysicsEntity() {
        if (b2Body_IsValid(bodyId)) b2DestroyBody(bodyId);
    }

    
    void update() override {
        updatePhysics();
    }

    
    void updatePhysics() {
        if (b2Body_IsValid(bodyId)) {
            b2Vec2 b2p = b2Body_GetPosition(bodyId);
            this->position = { b2p.x, b2p.y };

            b2Rot b2r = b2Body_GetRotation(bodyId);
            this->angle = b2Rot_GetAngle(b2r) * RAD2DEG;
        }
    }

    
    void setType(b2BodyType type) {
        if (b2Body_IsValid(bodyId)) {
            b2Body_SetType(bodyId, type);
            setAwake(true);
            b2Body_ApplyMassFromShapes(bodyId);
            debugStatus();
        }
    }


    void debugStatus() {
        if (!b2Body_IsValid(bodyId)) {
            printf("ERROR: ¡El body NO es válido!\n");
            return;
        }

        b2Vec2 p = b2Body_GetPosition(bodyId);
        b2BodyType t = b2Body_GetType(bodyId);
        bool awake = b2Body_IsAwake(bodyId);
        float mass = b2Body_GetMass(bodyId);

        printf("POS: %.2f, %.2f | TIPO: %d | AWAKE: %d | MASA: %.2f\n",
            p.x, p.y, (int)t, (int)awake, mass);
    }

    void setBodyPosition(Vector2 newPos) {
        if (!b2Body_IsValid(bodyId)) return;

        
        b2Body_SetTransform(bodyId, { newPos.x, newPos.y }, b2Rot_identity);

        
        b2Body_SetAwake(bodyId, true);

       
        this->position = newPos;
    }

    void applyImpulse(Vector2 impulse) {
        if (b2Body_IsValid(bodyId)) {
       
            b2Body_ApplyLinearImpulseToCenter(bodyId, { impulse.x, impulse.y }, true);
        }
    }

    void setAwake(bool awake) {
        if (b2Body_IsValid(bodyId)) {
            b2Body_SetAwake(bodyId, awake);
        }
    }

    void syncFromBody() {
        if (!b2Body_IsValid(bodyId)) return;
        b2Vec2 pos = b2Body_GetPosition(bodyId);
        position = { pos.x, pos.y };
        b2Rot rot = b2Body_GetRotation(bodyId);
        angle = b2Rot_GetAngle(rot) * RAD2DEG;
    }

};
