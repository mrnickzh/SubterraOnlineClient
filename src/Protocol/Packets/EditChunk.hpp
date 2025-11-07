#pragma once

#include "../Packet.hpp"
#include "../../main.hpp"
#include "../../Objects/AirObject.hpp"
#include "../../Objects/StaticObject.hpp"

class EditChunk : public Packet {
    public:
        Vector2 chunkpos;
        Vector2 blockpos;
        std::shared_ptr<Object> block;

    void receive(ByteBuf &buffer) override {
        chunkpos = Vector2(buffer.readFloat(), buffer.readFloat());
        Vector2 position;
        blockpos = Vector2(buffer.readFloat(), buffer.readFloat());
        position.x = (chunkpos.x * 1600.0f) + (blockpos.x * 100.0f);
        position.y = (chunkpos.y * 1600.0f) + (blockpos.y * 100.0f);
        int id = buffer.readInt();

        switch (id) {
            case 0: {
                std::shared_ptr<AirObject> airObject = std::make_shared<AirObject>(0.0f, position, Vector2(0.0f, 0.0f), Vector2(100.0f, 100.0f), false);
                block = airObject;
                break;
            }
            case 1: {
                std::shared_ptr<StaticObject> staticObject = std::make_shared<StaticObject>(0.0f, position, Vector2(0.0f, 0.0f), Vector2(100.0f, 100.0f), Main::dirtTexture, true);
                block = staticObject;
                break;
            }
            case 2: {
                std::shared_ptr<StaticObject> staticObject = std::make_shared<StaticObject>(0.0f, position, Vector2(0.0f, 0.0f), Vector2(100.0f, 100.0f), Main::stoneTexture, true);
                block = staticObject;
                break;
            }
        }

        Main::map[chunkpos]->addBlock(blockpos, block);
    }
    void send(ByteBuf &buffer) override {
        buffer.writeFloat(chunkpos.x);
        buffer.writeFloat(chunkpos.y);
        buffer.writeFloat(blockpos.x);
        buffer.writeFloat(blockpos.y);
        buffer.writeInt(0);
    }
};
