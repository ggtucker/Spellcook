#pragma once

#include "Core/CoreTypes.h"
#include "Keyboard.h"

namespace NInput {
    struct SSizeEvent {
        uint32_t m_width{};
        uint32_t m_height{};
    };
    struct SKeyEvent {
        NKeyboard::EKey m_key{NKeyboard::e_keyUnknown};
    };
    struct SMouseMoveEvent {
        int32_t m_x{};
        int32_t m_y{};
    };
    struct SMouseButtonEvent {

    };
    struct SMouseWheelEvent {

    };
}

class CInputSystem {
public:
    void ProcessEvent (const NInput::SSizeEvent& resize);
    void ProcessEvent (const NInput::SKeyEvent& resize);
    void ProcessEvent (const NInput::SMouseMoveEvent& resize);
    void ProcessEvent (const NInput::SMouseButtonEvent& resize);
    void ProcessEvent (const NInput::SMouseWheelEvent& resize);
private:
};