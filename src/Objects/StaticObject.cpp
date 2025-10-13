#include "StaticObject.hpp"

#include <rlgl.h>

#include "../main.hpp"

void StaticObject::render() {
    rlPushMatrix();
    rlTranslatef(position.x, position.y, 0);
    rlRotatef(rotation,0,0,-1);
    DrawTextureRec(texturedata, Rectangle(colliderAA.x, colliderAA.y, colliderBB.x, colliderBB.y), position, WHITE);
    rlPopMatrix();
}

void StaticObject::calculate_physics() {

}
