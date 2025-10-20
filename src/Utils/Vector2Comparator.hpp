#pragma once
#include <iostream>
#include <raylib.h>

struct Vector2Comparator {
    bool operator()(const Vector2& a, const Vector2& b) const {
        return std::tie(a.x, a.y) < std::tie(b.x, b.y);
    }
};
