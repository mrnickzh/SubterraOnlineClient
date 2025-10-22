#pragma once

#include "../Packet.hpp"
#include "../../main.hpp"

class PlayerAuthInput : public Packet {
public:
    void receive(ByteBuf &buffer) override {
       std::string uuid = buffer.readString();
       for (auto& e : Main::entities) {
           if (e->uuid == uuid) {
               e->object->position = Vector2(buffer.readFloat(), buffer.readFloat());
               e->object->rotation = buffer.readFloat();
               Main::physicsEngine->setVelocity(e->object, Vector2(buffer.readFloat(), buffer.readFloat()));
           }
       }
    }

    void send(ByteBuf &buffer) override {
        buffer.writeFloat(Main::localPlayer->object->position.x);
        buffer.writeFloat(Main::localPlayer->object->position.y);

        buffer.writeFloat(Main::localPlayer->object->rotation);

        Vector2 velocity = Main::physicsEngine->getVelocity(Main::localPlayer->object);
        buffer.writeFloat(velocity.x);
        buffer.writeFloat(velocity.y);
    }
};