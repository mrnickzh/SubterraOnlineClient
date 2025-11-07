#pragma once

#include <vector>

#include "InputHandler.hpp"
#include "Entities/Entity.hpp"
#include "Objects/Object.hpp"
#include "Event/event_dispatcher.hpp"
#include "Objects/ChunkMap.hpp"
#include "Objects/StaticObject.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Utils/Vector2Comparator.hpp"

namespace Main {
    inline std::vector<std::shared_ptr<Object>> objects;
    inline std::vector<std::shared_ptr<Object>> mapObjects;
    inline std::map<Vector2, std::shared_ptr<ChunkMap>, Vector2Comparator> map;

    inline std::unique_ptr<nes::event_dispatcher> mDispatcher;
    inline std::unique_ptr<PhysicsEngine> physicsEngine;

    inline Entity* localPlayer = nullptr;

    inline std::vector<std::shared_ptr<Entity>> entities;

    inline Texture2D playerTexture;
    inline Texture2D dirtTexture;
    inline Texture2D stoneTexture;

    inline bool chor = false;
    inline bool cver = false;
}
