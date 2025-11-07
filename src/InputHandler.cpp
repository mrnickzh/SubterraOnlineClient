#include "InputHandler.hpp"

#include "main.hpp"
#include "Event/Events/KeyEvent.hpp"
#include "Event/Events/MouseButtonEvent.hpp"
#include "Event/Events/MouseMoveEvent.hpp"

std::set<std::string> chars;

EM_BOOL InputHandler::mouseMoved(int, const EmscriptenMouseEvent* e, void*) {
    auto event = nes::make_holder<MouseMoveEvent>(ImVec2(e->clientX, e->clientY), ImVec2(e->movementX, e->movementY));
    Main::mDispatcher->trigger(event);
    if (event->canceled) return true;
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(e->clientX, e->clientY);
    return false;
}

EM_BOOL InputHandler::mouseButton(int eventType, const EmscriptenMouseEvent* e, void*) {
    auto event = nes::make_holder<MouseButtonEvent>(e->button, (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN));
    Main::mDispatcher->trigger(event);
    if (event->canceled) return true;
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(e->button, (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN));
    return false;
}

EM_BOOL InputHandler::keyPressed(int, const EmscriptenKeyboardEvent* e, void*) {
    chars.insert(std::string(e->code));
    auto event = nes::make_holder<KeyEvent>(std::string(e->code), true);
    Main::mDispatcher->trigger(event);
    if (event->canceled) return EM_TRUE;
    return EM_FALSE;
}

EM_BOOL InputHandler::keyReleased(int, const EmscriptenKeyboardEvent* e, void*) {
    chars.erase(std::string(e->code));
    auto event = nes::make_holder<KeyEvent>(std::string(e->code), false);
    Main::mDispatcher->trigger(event);
    if (event->canceled) return EM_TRUE;
    return EM_FALSE;
}

bool InputHandler::isKeyPressed(std::string key) {
    return chars.find(key) != chars.end();
}

bool InputHandler::isKeyReleased(std::string key) {
    return !(chars.find(key) != chars.end());
}