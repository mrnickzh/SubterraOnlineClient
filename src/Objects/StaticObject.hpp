#pragma once
#include <memory>
#include <raylib.h>

#include "Object.hpp"

class StaticObject : public Object {
public:
    Texture2D texturedata;
    StaticObject(float rotation, Vector2 position, Vector2 colliderAA, Vector2 colliderBB, Texture2D texture, bool canCollide) : Object(rotation, position, colliderAA, colliderBB, canCollide) {texturedata = texture;};

    void render() override;
};