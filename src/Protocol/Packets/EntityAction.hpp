#pragma once

#include "../Packet.hpp"
#include "../../main.hpp"
#include "../../Entities/Entity.hpp"

class EntityAction : public Packet {
public:
    void receive(ByteBuf &buffer) override {
        int action = buffer.readInt();
        std::string uuidStr = buffer.readString();

        if (action == 0) {
            std::shared_ptr<Entity> e = std::make_shared<Entity>();
            e->uuid = uuidStr;
            e->object = std::make_shared<DynamicObject>(0, Vector2(0, -100), Vector2(0, 0), Vector2(50, 50), Main::playerTexture, true);
            if (std::find_if(Main::entities.begin(), Main::entities.end(), [uuidStr](std::shared_ptr<Entity> &f)->bool{ return (*f).uuid == uuidStr; }) == Main::entities.end()) {
                Main::entities.push_back(e);
                Main::physicsEngine->registerObject(e->object, 1.0f);
                std::cout << e->uuid << " uuid" << std::endl;
                std::cout << Main::entities.size() << std::endl;
            }
        } else if (action == 1) {
            for (std::shared_ptr<Entity>& e : Main::entities) {
                if (e->uuid == uuidStr) {
                    Main::physicsEngine->unregisterObject(e->object);
                    Main::entities.erase(std::remove(Main::entities.begin(), Main::entities.end(), e), Main::entities.end());
                    break;
                }
            }
        }
    }

    void send(ByteBuf &buffer) override {}
};