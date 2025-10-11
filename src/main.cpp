#include "main.hpp"

#include <algorithm>
#include <emscripten.h>
#include <iostream>
#include <emscripten/html5.h>
#include <thread>
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_opengl3.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <queue>
#include <string>

#include "InputHandler.hpp"
#include "Event/Events/TickEvent.hpp"
#include "Protocol/PacketHelper.hpp"
#include "Protocol/Socket.hpp"
#include "Protocol/Packets/AddMapObject.hpp"
#include "Protocol/Packets/EntityAction.hpp"
#include "Protocol/Packets/HandShakePacket.hpp"
#include "Protocol/Packets/PlayerAuthInput.hpp"

int windowWidth = 800;
int windowHeight = 600;

Camera2D camera = { 0 };

EM_BOOL onResize(int, const EmscriptenUiEvent* e, void*) {
    windowWidth = e->windowInnerWidth;
    windowHeight = e->windowInnerHeight;
    SetWindowSize(windowWidth, windowHeight);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)e->windowInnerWidth, (float)e->windowInnerHeight);

    return EM_TRUE;
}

void setupAndRender() {
    // Main::objects.insert(Main::objects.end(), Main::map.begin(), Main::map.end());
    // Main::objects.push_back(Main::localPlayer->object);
    //
    // for (auto& e : Main::entities) {
    //     Main::objects.push_back(e->object);
    // }
    //
    // float camX = Main::localPlayer->object->position.x, camY = Main::localPlayer->object->position.y;
    // std::vector<std::shared_ptr<Object>> sortedObjects = Main::objects;
    // std::sort(sortedObjects.begin(), sortedObjects.end(),
    //           [camX, camY](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
    //               float distA = (a->position.x - camX)*(a->position.x - camX) +
    //                             (a->position.y - camY)*(a->position.y - camY);
    //               float distB = (b->position.x - camX)*(b->position.x - camX) +
    //                             (b->position.y - camY)*(b->position.y - camY);
    //               return distA > distB;
    //           });
    // for (const auto& object : sortedObjects) {
    //     object->render();
    // }
    // Main::objects.clear();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Test");
    ImGui::Text("Example test");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void mainLoop() {
    bool moved = false;

    // if (IsKeyDown(KEY_W)) {
    //     if (cv.z < 5.0f && cv.x < 5.0f) { Main::physicsEngine.add_dynamic_linear_velocity_with_rotation(bodyid, Vector3(0.1f, 0.1f, 0.1f), cr); moved = true; }
    // }
    // if (IsKeyDown(KEY_S)) {
    //     if (cv.z > -5.0f && cv.x > -5.0f) { Main::physicsEngine.add_dynamic_linear_velocity_with_rotation(bodyid, Vector3(-0.1f, -0.1f, -0.1f), cr); moved = true; }
    // }
    // if (IsKeyDown(KEY_A)) { Main::physicsEngine.set_dynamic_angular_velocity(bodyid, Vector3(0.0f, 20.0f, 0.0f)); moved = true; }
    // if (IsKeyDown(KEY_D)) { Main::physicsEngine.set_dynamic_angular_velocity(bodyid, Vector3(0.0f, -20.0f, 0.0f)); moved = true; }
    // if (moved) { PlayerAuthInput pkt; SocketClient::sendPacket(&pkt); std::cout << "PAI sent" << std::endl; }

    BeginDrawing();
    ClearBackground(GRAY);

    BeginMode2D(camera);

    DrawRectangle(50, 50, 20, 20, RED);

    setupAndRender();

    EndMode2D();
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
    //SocketClient::connect();

    InitWindow(windowWidth, windowHeight, "SubterraOnlineClient");

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

    emscripten_set_keydown_callback_on_thread(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, InputHandler::keyPressed, pthread_self());
    emscripten_set_keyup_callback_on_thread(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, InputHandler::keyReleased, pthread_self());
    emscripten_set_mousemove_callback_on_thread(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, InputHandler::mouseMoved, pthread_self());
    emscripten_set_mousedown_callback_on_thread(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, InputHandler::mouseButton, pthread_self());
    emscripten_set_mouseup_callback_on_thread(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, InputHandler::mouseButton, pthread_self());

    camera.target = (Vector2){ Main::localPlayer->object->position.x + 20.0f, Main::localPlayer->object->position.y + 20.0f };
    camera.offset = (Vector2){ windowWidth/2.0f, windowHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    std::thread tick([&]() {
        int tps = 40;
        int mps = 1000 / tps;
        long long ltt = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        while (ticklooprunning) {
            long long now = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            long long elapsed = now - ltt;

            if (elapsed >= mps) {
                int ticksToRun = (int) (elapsed / mps);
                ltt += ticksToRun * mps;

                for (int i = 0; i < ticksToRun; i++) {
                    auto event = nes::make_holder<TickEvent>();
                    Main::mDispatcher->trigger(event);
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
         }
    });

    emscripten_set_main_loop(mainLoop, 0, 1);
    ticklooprunning = false;
    tick.join();
    CloseWindow();

    return 0;
}

