#pragma once

#include "../Event.hpp"

class MouseMoveEvent : public Event {
public:
    ImVec2 pos;
    ImVec2 delta;

    MouseMoveEvent(ImVec2 pos, ImVec2 delta) : Event(), pos(pos), delta(delta) {

    }
};