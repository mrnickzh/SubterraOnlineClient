#include "main.hpp"

#include <algorithm>
#include <emscripten.h>
#include <iostream>
#include <emscripten/html5.h>
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_opengl3.h"

#include <array>
#include <cinttypes>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <queue>
#include <string>

#include "InputHandler.hpp"
#include "Event/Events/ConnectedEvent.hpp"
#include "Event/Events/KeyEvent.hpp"
#include "Event/Events/TickEvent.hpp"
#include "Protocol/PacketHelper.hpp"
#include "Protocol/Socket.hpp"
#include "Protocol/Packets/AddMapObject.hpp"
#include "Protocol/Packets/EntityAction.hpp"
#include "Protocol/Packets/GenerateChunk.hpp"
#include "Protocol/Packets/HandShakePacket.hpp"
#include "Protocol/Packets/PlayerAuthInput.hpp"

int windowWidth = 800;
int windowHeight = 600;

bool connected = false;

Camera2D camera = { 0 };

auto event = nes::make_holder<TickEvent>();
int tps = 60;
int mps = 1000 / tps;
long long ltt = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

void do_tick() {
    long long now = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    long long elapsed = now - ltt;

    if (elapsed >= mps) {
        int ticksToRun = (int) (elapsed / mps);
        for (int i = 0; i < ticksToRun; i++) {
            Main::mDispatcher->trigger(event);
        }
        ltt = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
}

EM_BOOL onResize(int, const EmscriptenUiEvent* e, void*) {
    windowWidth = e->windowInnerWidth;
    windowHeight = e->windowInnerHeight;
    SetWindowSize(windowWidth, windowHeight);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)e->windowInnerWidth, (float)e->windowInnerHeight);

    return EM_TRUE;
}

void preRender() {
    do_tick();

    Vector2 playerPos = Vector2(Main::localPlayer->object->position.x, Main::localPlayer->object->position.y);
    Vector2 playerChunk = Vector2(floor(playerPos.x / 1600.0f), floor(playerPos.y / 1600.0f));

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            Vector2 requestedChunk = Vector2(playerChunk.x + i, playerChunk.y + j);

            if (Main::map.find(requestedChunk) == Main::map.end()) {
                GenerateChunk pkt;
                pkt.chunkpos = requestedChunk;
                SocketClient::sendPacket(&pkt);
                continue;
            }

            std::shared_ptr<ChunkMap> chunk = Main::map[requestedChunk];
            std::map<Vector2, std::shared_ptr<Object>, Vector2Comparator> blocks = chunk->blocks;

            for (const auto& block : blocks) {
                if (sqrt((block.second->position.x - playerPos.x) * (block.second->position.x - playerPos.x) + (block.second->position.y - playerPos.y) * (block.second->position.y - playerPos.y)) < (float)std::max(windowHeight, windowWidth) * 1.25f) {
                    Main::objects.insert(Main::objects.end(), block.second);
                }
            }
        }
    }

    for (const auto& obj : Main::mapObjects) {
        if (sqrt((obj->position.x - playerPos.x) * (obj->position.x - playerPos.x) + (obj->position.y - playerPos.y) * (obj->position.y - playerPos.y)) < (float)std::max(windowHeight, windowWidth) * 1.25f) {
            Main::objects.insert(Main::objects.end(), obj);
        }
    }

    for (auto& e : Main::entities) {
        if (sqrt((e->object->position.x - playerPos.x) * (e->object->position.x - playerPos.x) + (e->object->position.y - playerPos.y) * (e->object->position.y - playerPos.y)) < (float)std::max(windowHeight, windowWidth) * 1.25f) {
            Main::objects.insert(Main::objects.end(), e->object);
        }
    }

    float camX = playerPos.x, camY = playerPos.y;
    std::sort(Main::objects.begin(), Main::objects.end(),
              [camX, camY](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
                  float distA = sqrt(std::abs((a->position.x - camX)*(a->position.x - camX) +
                                (a->position.y - camY)*(a->position.y - camY)));
                  float distB = sqrt(std::abs((b->position.x - camX)*(b->position.x - camX) +
                                (b->position.y - camY)*(b->position.y - camY)));
                  return distA < distB;
              });
}

void actualRender() {
    for (const auto& object : Main::objects) {
        object->render();
    }

    Main::localPlayer->object->render();
    camera.target = Main::localPlayer->object->position;
}

void postRender() {
    Main::objects.clear();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Test");
    ImGui::Text("Hello World");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    Vector2 vel = Main::physicsEngine->getVelocity(Main::localPlayer->object);
    DrawText(("FPS: " + std::to_string(GetFPS())).c_str(), 10, 10, 10, BLACK);
    DrawText(("X: " + std::to_string(Main::localPlayer->object->position.x) + " Y: " + std::to_string(Main::localPlayer->object->position.y)).c_str(), 10, 20, 10, BLACK);
    DrawText(("vX: " + std::to_string(vel.x) + " vY: " + std::to_string(vel.y)).c_str(), 10, 30, 10, BLACK);
}

void mainLoop() {
    if (!connected) {
        return;
    }

    bool moved = false;
    Vector2 playerVelocity = Main::physicsEngine->getVelocity(Main::localPlayer->object);

    if (Main::inputHandler.isKeyPressed("KeyD")) { Main::physicsEngine->addVelocityClamped(Main::localPlayer->object, Vector2(0.5f, 0.0f), Vector2(-2.0f, 2.0f), Vector2(-5.0f, 100.0f)); moved = true; }
    if (Main::inputHandler.isKeyPressed("KeyA")) { Main::physicsEngine->addVelocityClamped(Main::localPlayer->object, Vector2(-0.5f, 0.0f), Vector2(-2.0f, 2.0f), Vector2(-5.0f, 100.0f)); moved = true; }
    if (Main::inputHandler.isKeyPressed("Space")) {
        if (playerVelocity.y == 0.0f) { Main::physicsEngine->addVelocityClamped(Main::localPlayer->object, Vector2(0.0f, -5.0f), Vector2(-2.0f, 2.0f), Vector2(-5.0f, 100.0f)); moved = true; }
    }
    if (moved) { PlayerAuthInput pkt; SocketClient::sendPacket(&pkt); }

    BeginDrawing();
    ClearBackground(WHITE);

    preRender();

    BeginMode2D(camera);

    actualRender();

    EndMode2D();

    postRender();

    EndDrawing();
}

int main() {
    bool ticklooprunning = true;

    emscripten_get_canvas_element_size("#canvas", &windowWidth, &windowHeight);
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, onResize);
    emscripten_request_pointerlock("canvas", EM_TRUE);

    PacketHelper::registerPacket(0, []() { return new HandShakePacket(); });
    PacketHelper::registerPacket(1, []() { return new AddMapObject(); });
    PacketHelper::registerPacket(2, []() { return new EntityAction(); });
    PacketHelper::registerPacket(3, []() { return new PlayerAuthInput(); });
    PacketHelper::registerPacket(4, []() { return new GenerateChunk(); });
    SocketClient::connect();

    SetTraceLogLevel(LOG_ERROR);
    InitWindow(windowWidth, windowHeight, "SubterraOnlineClient");
    SetTargetFPS(60);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);
    ImGui::StyleColorsDark();
    ImFont* customFont = io.Fonts->AddFontFromFileTTF("assets/fonts/montserrat.ttf", 16.0f);
    if (!customFont) {
        printf("Fuck font");
    }
    ImGui_ImplOpenGL3_Init("#version 300 es");

    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, InputHandler::keyPressed);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, InputHandler::keyReleased);
    emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, InputHandler::mouseMoved);
    emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, InputHandler::mouseButton);
    emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, InputHandler::mouseButton);

    camera.target = Vector2(0.0f, 0.0f);
    camera.offset = (Vector2){ windowWidth/2.0f, windowHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Image image = GenImageColor(50, 50, RED);
    Main::playerTexture = LoadTextureFromImage(image);
    image = GenImageColor(100, 100, BROWN);
    Main::dirtTexture = LoadTextureFromImage(image);
    image = GenImageColor(100, 100, GRAY);
    Main::stoneTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    Main::localPlayer = new Entity();
    Main::localPlayer->object = std::make_shared<DynamicObject>(0, Vector2(0, -100), Vector2(0, 0), Vector2(50, 50), Main::playerTexture, true);

    Main::physicsEngine = std::make_unique<PhysicsEngine>(&Main::map);
    Main::physicsEngine->registerObject(Main::localPlayer->object, 1.0f);

    auto connectedCallback = [](ConnectedEvent& event) {
        connected = true;
        HandShakePacket pkt;
        pkt.name = "playername";
        SocketClient::sendPacket(&pkt);
    };
    Main::mDispatcher->listen<ConnectedEvent>(connectedCallback);

    auto tickCallback = [](TickEvent& event) {
        Main::physicsEngine->step();
    };
    Main::mDispatcher->listen<TickEvent>(tickCallback);

    emscripten_set_main_loop(mainLoop, 0, 1);

    CloseWindow();

    return 0;
}

