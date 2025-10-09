#pragma once
#include <memory>
#include <raylib.h>

#include "Object.hpp"

class DynamicObject : public Object {
public:
    Texture2D texturedata;
    DynamicObject(Vector2 rotation, Vector2 position, Vector2 colliderAA, Vector2 colliderBB, Texture2D texture) : Object(rotation, position, colliderAA, colliderBB) {texturedata = texture;};

    void render() override;
    void calculate_physics() override;
};