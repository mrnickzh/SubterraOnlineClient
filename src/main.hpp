#pragma once

#include <vector>

#include "Entities/Entity.hpp"
#include "Objects/Object.hpp"
#include "Event/event_dispatcher.hpp"

namespace Main {
    inline std::vector<std::shared_ptr<Object>> objects;
    inline std::vector<std::shared_ptr<Object>> map;

    inline std::unique_ptr<nes::event_dispatcher> mDispatcher;

    inline Entity* localPlayer = nullptr;

    inline std::vector<std::shared_ptr<Entity>> entities;
}
