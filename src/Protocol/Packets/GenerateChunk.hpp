#pragma once

#include "../Packet.hpp"
#include "../../main.hpp"
#include "../../Objects/AirObject.hpp"
#include "../../Objects/StaticObject.hpp"

class GenerateChunk : public Packet {
    public:
        Vector2 chunkpos;

    void receive(ByteBuf &buffer) override {
        std::shared_ptr<ChunkMap> chunkMap = std::make_shared<ChunkMap>();
        chunkpos = Vector2(buffer.readFloat(), buffer.readFloat());

        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                int id = buffer.readInt();
                Vector2 position;
                Vector2 blockPos = Vector2(buffer.readFloat(), buffer.readFloat());
                position.x = (chunkpos.x * 1600.0f) + (blockPos.x * 100.0f);
                position.y = (chunkpos.y * 1600.0f) + (blockPos.y * 100.0f);

                switch (id) {
                    case 0: {
                        std::shared_ptr<AirObject> airObject = std::make_shared<AirObject>(0.0f, position, Vector2(0.0f, 0.0f), Vector2(100.0f, 100.0f), false);
                        chunkMap->addBlock(Vector2(blockPos.x, blockPos.y), airObject);
                        break;
                    }
                    case 1: {
                        std::shared_ptr<StaticObject> staticObject = std::make_shared<StaticObject>(0.0f, position, Vector2(0.0f, 0.0f), Vector2(100.0f, 100.0f), Main::dirtTexture, true);
                        chunkMap->addBlock(Vector2(blockPos.x, blockPos.y), staticObject);
                        break;
                    }
                    case 2: {
                        std::shared_ptr<StaticObject> staticObject = std::make_shared<StaticObject>(0.0f, position, Vector2(0.0f, 0.0f), Vector2(100.0f, 100.0f), Main::stoneTexture, true);
                        chunkMap->addBlock(Vector2(blockPos.x, blockPos.y), staticObject);
                        break;
                    }
                }
            }
        }

        Main::map[chunkpos] = chunkMap;
    }
    void send(ByteBuf &buffer) override {
        buffer.writeFloat(chunkpos.x);
        buffer.writeFloat(chunkpos.y);
    }
};
