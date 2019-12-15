#pragma once

#define EVENT_CLASS_TYPE(type)  static EventType GetTypeStatic() { return EventType::type; }\
							    virtual EventType GetType() const override { return GetTypeStatic(); }

namespace Oasis
{
    ////////////////////////////////////////////
    enum class EventType
    {
        NONE = 0,

        // WINDOW EVENTS
        WINDOW_CLOSE,
        WINDOW_SIZE_CHANGED,

        // KEY EVENTS
        KEY_PRESSED,
        KEY_RELEASED,

        // MOUSE EVENTS
        MOUSE_PRESS,
        MOUSE_RELEASE,
        MOUSE_MOVE,
        MOUSE_SCROLL,

        // OTHER EVENTS
        TEXT_INPUT
    };

    ////////////////////////////////////////////
    class Event
    {
    public:
        // Allow events to return static typing information
        virtual EventType GetType() const = 0;
    };
}