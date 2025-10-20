#pragma once
#include <map>
#include <memory>
#include <raylib.h>

#include "Object.hpp"
#include "../Utils/Vector2Comparator.hpp"

class ChunkMap {
public:
    std::map<Vector2, std::shared_ptr<Object>, Vector2Comparator> blocks;

    void addBlock(Vector2 blockPos, std::shared_ptr<Object> block);
    std::shared_ptr<Object> getBlock(Vector2 blockPos);
};
