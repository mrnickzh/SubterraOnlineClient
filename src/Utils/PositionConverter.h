#pragma once
#include <raylib.h>

namespace PositionConverter {
    inline Vector2 C2TL(Vector2 position, Vector2 size) {
        return Vector2(position.x + (size.x / 2), position.y + (size.y / 2));
    };

    inline Vector2 S2WP(Vector2 point, Vector2 window, Vector2 position) {
        return Vector2(point.x - (window.x / 2) + position.x, point.y - (window.y / 2) + position.y);
    }
}
