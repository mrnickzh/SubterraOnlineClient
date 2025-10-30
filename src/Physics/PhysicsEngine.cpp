#include "PhysicsEngine.hpp"

#include "../Objects/AirObject.hpp"

PhysicsEngine::PhysicsEngine(std::map<Vector2, std::shared_ptr<ChunkMap>, Vector2Comparator>* worldmap) {
    chunkmap = worldmap;
}

bool PhysicsEngine::isColliding(Vector2 object1, Vector2 object2, Vector2 collider1, Vector2 collider2) {
    bool xcheck1 = object1.x <= object2.x + collider2.x;
    bool x1check1 = object1.x + collider1.x >= object2.x;
    bool ycheck1 = object1.y <= object2.y + collider2.y;
    bool y1check1 = object1.y + collider1.y >= object2.y;
    if (xcheck1 && ycheck1 && x1check1 && y1check1) {
        // std::cout << "====================" << std::endl;
        // std::cout << object1.x << " " << object1.y << std::endl;
        // std::cout << collider1.x << " " << collider1.y << std::endl;
        // std::cout << object2.x << " " << object2.y << std::endl;
        // std::cout << collider2.x << " " << collider2.y << std::endl;
        // std::cout << "====================" << std::endl;
        return true;
    }
    return false;
}

bool PhysicsEngine::possibleCollision(Vector2 position, Vector2 collider, std::shared_ptr<Object>& object2) {
    if (!object2->canCollide) { return false; }
    return isColliding(position, object2->position, collider, object2->colliderBB);
}

std::vector<std::shared_ptr<Object>> PhysicsEngine::possibleObstacles(Vector2 position) {
    std::vector<std::shared_ptr<Object>> obstacles;
    Vector2 currentChunk = Vector2(floor(position.x / 1600.0f), floor(position.y / 1600.0f));
    Vector2 currentChunkBlock = Vector2(floor(std::fmod(position.x, 1600.0f) / 100.0f), floor(std::fmod(position.y, 1600.0f) / 100.0f));
    currentChunkBlock.x += (currentChunk.x < 0.0f ? (currentChunkBlock.x == 0.0f ? 0.0f : 16.0f) : 0.0f);
    currentChunkBlock.y += (currentChunk.y < 0.0f ? (currentChunkBlock.y == 0.0f ? 0.0f : 16.0f) : 0.0f);
    for (int i = (int)currentChunkBlock.x - 1; i <= (int)currentChunkBlock.x + 1; i++) {
        Vector2 collisionChunk = currentChunk;
        Vector2 collisionChunkBlock = currentChunkBlock;
        if (i < 0) { collisionChunk.x -= 1.0f; collisionChunkBlock.x = 15.0f; }
        if (i > 15) { collisionChunk.x += 1.0f; collisionChunkBlock.x = 0.0f; }
        for (int j = (int)currentChunkBlock.y - 1; j <= (int)currentChunkBlock.y + 1; j++) {
            if (j < 0) { collisionChunk.y -= 1.0f; collisionChunkBlock.y = 15.0f; }
            if (j > 15) { collisionChunk.y += 1.0f; collisionChunkBlock.y = 0.0f; }
            if ((*chunkmap).find(collisionChunk) == (*chunkmap).end()) { return obstacles; }
            obstacles.push_back((*chunkmap)[collisionChunk]->getBlock(collisionChunkBlock));
        }
    }
    return obstacles;
}

void PhysicsEngine::calculateVelocity(std::shared_ptr<PhysicsObject>& obj) {
    Vector2 vel = obj->velocity;
    Vector2 pos = obj->getPosition();

    bool verticalCollision = false;
    bool horizontalCollision = false;

    std::vector<std::shared_ptr<Object>> obstacles = possibleObstacles(pos);
    if (obstacles.size() < 9) { std::cout << "not loaded" << std::endl; return; }

    float inertiaAdjusted = 0.05f * obj->mass;
    float gravityAdjusted = 0.1f * obj->mass;

    if (vel.x < 0) { vel.x += inertiaAdjusted; }
    if (vel.x > 0) { vel.x -= inertiaAdjusted; }
    if (vel.x < inertiaAdjusted && vel.x > -inertiaAdjusted) { vel.x = 0; }

    vel.y += gravityAdjusted;

    for (auto obstacle : obstacles) {
        if (possibleCollision(Vector2(pos.x + vel.x, pos.y), obj->getCollider(), obstacle) && horizontalCollision != true) { horizontalCollision = true; }
        if (possibleCollision(Vector2(pos.x, pos.y + vel.y), obj->getCollider(), obstacle) && verticalCollision != true) { verticalCollision = true; }
    }

    if (!horizontalCollision) { pos.x += vel.x; }
    else { vel.x = 0.0f; }
    if (!verticalCollision) { pos.y += vel.y; }
    else { vel.y = 0.0f; }

    // std::cout << !horizontalCollision << std::endl;
    // std::cout << !verticalCollision << std::endl;
    // std::cout << pos.x << " " << pos.y << std::endl;
    // std::cout << vel.x << " " << vel.y << std::endl;

    obj->setPosition(pos);
    obj->velocity = vel;
}

void PhysicsEngine::registerObject(std::shared_ptr<Object> object, float mass) {
    std::shared_ptr<PhysicsObject> physicsObject = std::make_shared<PhysicsObject>(object, mass);
    registeredObjects.push_back(physicsObject);
}

void PhysicsEngine::unregisterObject(std::shared_ptr<Object> object) {
    auto it = std::find_if(registeredObjects.begin(), registeredObjects.end(), [&object](const std::shared_ptr<PhysicsObject>& obj) {return obj->object == object; });
    if (it != registeredObjects.end()) { registeredObjects.erase(it); }
}

void PhysicsEngine::addVelocity(std::shared_ptr<Object> object, Vector2 velocity) {
    auto it = std::find_if(registeredObjects.begin(), registeredObjects.end(), [&object](const std::shared_ptr<PhysicsObject>& obj) {return obj->object == object; });
    if (it != registeredObjects.end()) { it->get()->velocity = Vector2(it->get()->velocity.x + velocity.x, it->get()->velocity.y + velocity.y); }
}

void PhysicsEngine::addVelocityClamped(std::shared_ptr<Object> object, Vector2 velocity, Vector2 limitX, Vector2 limitY) {
    auto it = std::find_if(registeredObjects.begin(), registeredObjects.end(), [&object](const std::shared_ptr<PhysicsObject>& obj) {return obj->object == object; });
    if (it != registeredObjects.end()) { it->get()->velocity = Vector2(std::clamp(it->get()->velocity.x + velocity.x, limitX.x, limitX.y), std::clamp(it->get()->velocity.y + velocity.y, limitY.x, limitY.y)); }
}

void PhysicsEngine::setVelocity(std::shared_ptr<Object> object, Vector2 velocity) {
    auto it = std::find_if(registeredObjects.begin(), registeredObjects.end(), [&object](const std::shared_ptr<PhysicsObject>& obj) {return obj->object == object; });
    if (it != registeredObjects.end()) { it->get()->velocity = velocity; }
}

Vector2 PhysicsEngine::getVelocity(std::shared_ptr<Object> object) {
    auto it = std::find_if(registeredObjects.begin(), registeredObjects.end(), [&object](const std::shared_ptr<PhysicsObject>& obj) {return obj->object == object; });
    if (it != registeredObjects.end()) { return it->get()->velocity; }
    else { return Vector2(0.0f, 0.0f); }
}

void PhysicsEngine::step() {
    for (std::shared_ptr<PhysicsObject> object : registeredObjects) {
        calculateVelocity(object);
    }
}