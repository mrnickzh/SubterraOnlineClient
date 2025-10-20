#pragma once

#include "../Packet.hpp"
#include "../../main.hpp"

class PlayerAuthInput : public Packet {
public:
    void receive(ByteBuf &buffer) override {
       // std::string uuid = buffer.readString();
       // for (auto& e : Main::entities) {
       //     if (e->uuid == uuid) {
       //         auto epos = Vector3(buffer.readFloat(), buffer.readFloat(), buffer.readFloat());
       //         auto erot = Vector3(buffer.readFloat(), buffer.readFloat(), buffer.readFloat());
       //         auto evel = Vector3(buffer.readFloat(), buffer.readFloat(), buffer.readFloat());
       //     }
       // }
    }

    void send(ByteBuf &buffer) override {
        buffer.writeFloat(Main::localPlayer->object->position.x);
        buffer.writeFloat(Main::localPlayer->object->position.y);

        buffer.writeFloat(Main::localPlayer->object->rotation);

        buffer.writeFloat(0.0f);
        buffer.writeFloat(0.0f);
    }
};