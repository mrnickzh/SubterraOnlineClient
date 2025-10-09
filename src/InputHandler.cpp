#include "InputHandler.hpp"

#include "main.hpp"
#include "Event/Events/KeyEvent.hpp"
#include "Event/Events/MouseMoveEvent.hpp"

std::set<char> chars;

bool InputHandler::mouseMoved(int, const EmscriptenMouseEvent* e, void*) {
    auto event = nes::make_holder<MouseMoveEvent>(ImVec2(e->clientX, e->clientY), ImVec2(e->movementX, e->movementY));
    Main::mDispatcher->trigger(event);
    if (event->canceled) return true;
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(e->clientX, e->clientY);
    return false;
}

bool InputHandler::mouseButton(int eventType, const EmscriptenMouseEvent* e, void*) {
    ImGuiIO& io = ImGui::GetIO();
    int button = e->button;
    bool down = (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN);
    io.AddMouseButtonEvent(button, down);
    return false;
}

EM_BOOL InputHandler::keyPressed(int, const EmscriptenKeyboardEvent* e, void*) {
    chars.insert(e->keyCode);
    auto event = nes::make_holder<KeyEvent>(e->keyCode, true);
    Main::mDispatcher->trigger(event);
    if (event->canceled) return EM_TRUE;
    return EM_FALSE;
}

EM_BOOL InputHandler::keyReleased(int, const EmscriptenKeyboardEvent* e, void*) {
    chars.erase(e->keyCode);
    auto event = nes::make_holder<KeyEvent>(e->keyCode, false);
    Main::mDispatcher->trigger(event);
    if (event->canceled) return EM_TRUE;
    return EM_FALSE;
}

bool InputHandler::isKeyPressed(int key) {
    return chars.find(key) != chars.end();
}

bool InputHandler::isKeyReleased(int key) {
    return !(chars.find(key) != chars.end());
}