#pragma once

#include "events/event.hpp"

namespace Oasis
{
    class WindowCloseEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(WINDOW_CLOSE);
    };

    class WindowSizeChangedEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(WINDOW_SIZE_CHANGED);

        WindowSizeChangedEvent(int width, int height) : m_width(width), m_height(height) {}
        inline int GetWidth() const { return m_width; }
        inline int GetHeight() const { return m_height; }

    private:
        int m_width;
        int m_height;
    };
}