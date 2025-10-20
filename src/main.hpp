#pragma once

#include <vector>

#include "Entities/Entity.hpp"
#include "Objects/Object.hpp"
#include "Event/event_dispatcher.hpp"
#include "Objects/ChunkMap.hpp"
#include "Utils/Vector2Comparator.hpp"

namespace Main {
    inline std::vector<std::shared_ptr<Object>> objects;
    inline std::vector<std::shared_ptr<Object>> mapObjects;
    inline std::map<Vector2, std::shared_ptr<ChunkMap>, Vector2Comparator> map;

    inline std::unique_ptr<nes::event_dispatcher> mDispatcher;

    inline Entity* localPlayer = nullptr;

    inline std::vector<std::shared_ptr<Entity>> entities;
}
