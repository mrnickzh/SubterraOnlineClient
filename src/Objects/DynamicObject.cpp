#include "DynamicObject.hpp"

#include <rlgl.h>

#include "../main.hpp"

void DynamicObject::render() {
    rlPushMatrix();
    rlTranslatef(50,50, 0);
    rlRotatef(30,0,0,-1);
    DrawTextureRec(texturedata, Rectangle(colliderAA.x, colliderAA.y, colliderBB.x, colliderBB.y), position, WHITE);
    rlPopMatrix();
}

void DynamicObject::calculate_physics() {

}
