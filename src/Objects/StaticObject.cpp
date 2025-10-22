#include "StaticObject.hpp"

#include "../Utils/PositionConverter.h"

void StaticObject::render() {
    DrawTexturePro(texturedata, Rectangle(colliderAA.x, colliderAA.y, colliderBB.x, colliderBB.y), Rectangle(PositionConverter::C2TL(position, colliderBB).x, PositionConverter::C2TL(position, colliderBB).y, colliderBB.x, colliderBB.y), Vector2(colliderBB.x / 2, colliderBB.y /2), rotation, WHITE);
}
