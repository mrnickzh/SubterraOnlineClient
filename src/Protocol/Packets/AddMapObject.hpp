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
                std::shared_ptr<AirObject> airObject = std::make_shared<AirObject>(rotation, position, Vector2(0.0f, 0.0f), Vector2(100.0f, 100.0f));
                Main::mapObjects.push_back(airObject);
                break;
            }
            case 1: {
                Image dirtimage;
                Texture2D texture;
                dirtimage = GenImageColor(100, 100, BROWN);
                texture = LoadTextureFromImage(dirtimage);
                std::shared_ptr<StaticObject> staticObject = std::make_shared<StaticObject>(rotation, position, Vector2(0.0f, 0.0f), Vector2(100.0f, 100.0f), texture);
                //staticObject->create_physics();
                Main::mapObjects.push_back(staticObject);
                break;
            }
            case 2: {
                Image dirtimage;
                Texture2D texture;
                dirtimage = GenImageColor(100, 100, GRAY);
                texture = LoadTextureFromImage(dirtimage);
                std::shared_ptr<StaticObject> staticObject = std::make_shared<StaticObject>(rotation, position, Vector2(0.0f, 0.0f), Vector2(100.0f, 100.0f), texture);
                //staticObject->create_physics();
                Main::mapObjects.push_back(staticObject);
                break;
            }
        }
    }
    void send(ByteBuf &buffer) override {}
};
