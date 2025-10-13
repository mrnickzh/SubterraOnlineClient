#pragma once

#include <raylib.h>

class Object {
public:
    float rotation;
    Vector2 position;
    Vector2 colliderAA;
    Vector2 colliderBB;
    Object(float rotation, Vector2 position, Vector2 colliderAA, Vector2 colliderBB) : rotation(rotation), position(position), colliderAA(colliderAA), colliderBB(colliderBB) {}

    virtual void render() {}
    virtual void calculate_physics() {}
};
