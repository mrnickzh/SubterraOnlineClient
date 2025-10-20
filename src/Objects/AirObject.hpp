#pragma once
#include <memory>
#include <raylib.h>

#include "Object.hpp"

class AirObject : public Object {
public:
    AirObject(float rotation, Vector2 position, Vector2 colliderAA, Vector2 colliderBB) : Object(rotation, position, colliderAA, colliderBB) {};

    void render() override;
    void calculate_physics() override;
};