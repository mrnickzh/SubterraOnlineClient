#pragma once
#include <raylib.h>

namespace PositionConverter {
    inline Vector2 C2TL(Vector2 position, Vector2 size) {
        return Vector2(position.x + (size.x / 2), position.y + (size.y / 2));
    };
}
