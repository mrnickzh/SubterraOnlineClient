#include "DynamicObject.hpp"

#include <raymath.h>

#include "../main.hpp"

void DynamicObject::render() {
    DrawTextureRec(texturedata, Rectangle(0, 0, 50, 50), position, WHITE);
}

void DynamicObject::calculate_physics() {

}
