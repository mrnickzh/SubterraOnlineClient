#pragma once

#include <raylib.h>

class Object {
public:
    float rotation;
    bool canCollide;
    Vector2 position;
    Vector2 colliderAA;
    Vector2 colliderBB;
    Object(float rotation, Vector2 position, Vector2 colliderAA, Vector2 colliderBB, bool canCollide) : rotation(rotation), position(position), colliderAA(colliderAA), colliderBB(colliderBB), canCollide(canCollide) {}

    virtual void render() {}
};
