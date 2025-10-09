#include "StaticObject.hpp"

#include <raymath.h>

#include "../main.hpp"

void StaticObject::render() {
    DrawTextureRec(texturedata, Rectangle(0, 0, 50, 50), position, WHITE);
}

void StaticObject::calculate_physics() {

}
