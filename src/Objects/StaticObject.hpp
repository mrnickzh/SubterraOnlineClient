#pragma once
#include <memory>
#include <raylib.h>

#include "Object.hpp"

class StaticObject : public Object {
public:
    Texture2D texturedata;
    StaticObject(Vector2 rotation, Vector2 position, Vector2 colliderAA, Vector2 colliderBB, Texture2D texture) : Object(rotation, position, colliderAA, colliderBB) {texturedata = texture;};

    void render() override;
    void calculate_physics() override;
};