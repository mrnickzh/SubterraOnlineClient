#pragma once
#include <vector>

#include "../Objects/ChunkMap.hpp"

#include "PhysicsObject.hpp"

class PhysicsEngine {
public:
    std::vector<std::shared_ptr<PhysicsObject>> registeredObjects;
    std::map<Vector2, std::shared_ptr<ChunkMap>, Vector2Comparator>* chunkmap;

    PhysicsEngine(std::map<Vector2, std::shared_ptr<ChunkMap>, Vector2Comparator>* worldmap);
    void registerObject(std::shared_ptr<Object> object, float mass);
    void unregisterObject(std::shared_ptr<Object> object);
    void addVelocity(std::shared_ptr<Object> object, Vector2 velocity);
    void addVelocityClamped(std::shared_ptr<Object> object, Vector2 velocity, Vector2 limitX, Vector2 limitY);
    void setVelocity(std::shared_ptr<Object> object, Vector2 velocity);
    Vector2 getVelocity(std::shared_ptr<Object> object);
    void step();
private:
    bool isColliding(Vector2 object1, Vector2 object2, Vector2 collider1, Vector2 collider2);
    bool possibleCollision(Vector2 position, Vector2 collider, std::shared_ptr<Object>& object2);
    std::vector<std::shared_ptr<Object>> possibleObstacles(Vector2 position);
    void calculateVelocity(std::shared_ptr<PhysicsObject>& obj);
};
