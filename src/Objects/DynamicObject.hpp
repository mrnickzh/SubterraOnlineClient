#pragma once
#include <memory>
#include <raylib.h>

#include "Object.hpp"

class DynamicObject : public Object {
public:
    Texture2D texturedata;
    DynamicObject(float rotation, Vector2 position, Vector2 colliderAA, Vector2 colliderBB, Texture2D texture, bool canCollide) : Object(rotation, position, colliderAA, colliderBB, canCollide) {texturedata = texture;};

    void render() override;
};