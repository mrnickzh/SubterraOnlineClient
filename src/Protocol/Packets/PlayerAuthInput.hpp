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

        // buffer.writeFloat(position.x);
        // buffer.writeFloat(position.y);
        // buffer.writeFloat(position.z);
        //
        // buffer.writeFloat(rotation.x);
        // buffer.writeFloat(rotation.y);
        // buffer.writeFloat(rotation.z);
        //
        // buffer.writeFloat(velocity.x);
        // buffer.writeFloat(velocity.y);
        // buffer.writeFloat(velocity.z);
    }
};