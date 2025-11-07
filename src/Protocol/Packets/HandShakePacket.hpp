#pragma once

#include "../Packet.hpp"
#include "../../main.hpp"

class HandShakePacket : public Packet {

public:
    std::string name;

    void send(ByteBuf &buffer) override {
        buffer.writeString(name);
    }
    void receive(ByteBuf &buffer) override {}
};