#pragma once

#include "Core/CoreTypes.h"
#include "Keyboard.h"
#include "Mouse.h"

class CInputEvent {
public:
    struct SResizeEvent {
        uint32_t m_width;
        uint32_t m_height;
    };

    struct SKeyEvent {
        NKeyboard::EKey m_key;
        bool m_shift;
        bool m_control;
        bool m_alt;
        bool m_system;
    };

    struct SMouseButtonEvent {
        NMouse::EButton m_button;
        int32_t m_x;
        int32_t m_y;
    };

    struct SMouseMoveEvent {
        int32_t m_x;
        int32_t m_y;
    };

    struct SMouseScrollEvent {
        int32_t m_x;
        int32_t m_y;
    };

    enum class EEventType {
        Closed,
        Resized,
        KeyPressed,
        KeyReleased,
        MouseScrolled,
        MousePressed,
        MouseReleased,
        MouseMoved,
        MouseEntered,
        MouseLeft,
        Count
    };

    EEventType m_type{};

    union {
        SResizeEvent m_resize;
        SKeyEvent m_key;
        SMouseButtonEvent m_mouseButton;
        SMouseMoveEvent m_mouseMove;
        SMouseScrollEvent m_mouseScroll;
    };
};