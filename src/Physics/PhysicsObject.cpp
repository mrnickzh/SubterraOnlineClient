#include "PhysicsObject.hpp"

PhysicsObject::PhysicsObject(std::shared_ptr<Object> obj, float m) {
    object = obj;
    mass = m;
    velocity = Vector2(0.0f, 0.0f);
}

Vector2 PhysicsObject::getPosition() { return object->position; }
void PhysicsObject::setPosition(Vector2 pos) { object->position = pos; }
Vector2 PhysicsObject::getCollider() { return object->colliderBB; }
