#pragma once

#include "../Event.hpp"

class SetupAndRenderEvent : public Event {
public:
    enum Type {
        PRE_SHADER,
        PRE_RENDER,
        POST_RENDER
    };
    Type type;

    SetupAndRenderEvent(Type type) : Event(), type(type) {}
};