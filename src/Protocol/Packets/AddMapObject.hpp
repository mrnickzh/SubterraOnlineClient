#pragma once

#include "../Packet.hpp"
#include "../../main.hpp"
#include "../../Objects/AirObject.hpp"
#include "../../Objects/StaticObject.hpp"

class AddMapObject : public Packet {
    void receive(ByteBuf &buffer) override {
        int id = buffer.readInt();
        float rotation;
        Vector2 position;
        position.x = buffer.readFloat();
        position.y = buffer.readFloat();
        rotation = buffer.readFloat();

        switch (id) {
            case 0: {
                std::shared_ptr<AirObject> airObject = std::make_shared<AirObject>(rotation, position, Vector2(0.0f, 0.0f), Vector2(100.0f, 100.0f), false);
                Main::mapObjects.push_back(airObject);
                break;
            }
            case 1: {
                std::shared_ptr<StaticObject> staticObject = std::make_shared<StaticObject>(rotation, position, Vector2(0.0f, 0.0f), Vector2(100.0f, 100.0f), Main::dirtTexture, true);
                Main::mapObjects.push_back(staticObject);
                break;
            }
            case 2: {
                std::shared_ptr<StaticObject> staticObject = std::make_shared<StaticObject>(rotation, position, Vector2(0.0f, 0.0f), Vector2(100.0f, 100.0f), Main::stoneTexture, true);
                Main::mapObjects.push_back(staticObject);
                break;
            }
        }
    }
    void send(ByteBuf &buffer) override {}
};
