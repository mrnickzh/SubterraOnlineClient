#include "ChunkMap.hpp"


void ChunkMap::addBlock(Vector2 blockPos, std::shared_ptr<Object> block) {
    blocks[blockPos] = block;
}

std::shared_ptr<Object> ChunkMap::getBlock(Vector2 blockPos) {
    return blocks[blockPos];
}

