#pragma once
#include "../Objects/Object.hpp"
#include "../Utils/Vector2Comparator.hpp"

class PhysicsObject {
public:
    std::shared_ptr<Object> object;
    Vector2 velocity;
    float mass;

    PhysicsObject(std::shared_ptr<Object>& obj, float m);
    Vector2 getPosition();
    void setPosition(Vector2 pos);
    Vector2 getCollider();
};

