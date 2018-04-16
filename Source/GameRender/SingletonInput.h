#pragma once

#include <bitset>
#include "Core/Fixed.h"
#include "Math/Vector2.h"
#include "GameRender/Hotkey.h"
#include "Render/InputEvent.h"

class CInputHandler;

struct SSingletonInput {
    // Keyboard state
    bool IsHotkeyPressed (NHotkey::EHotkey hotkey) const { return m_hotkeysPressed.test(hotkey); }
    bool IsHotkeyReleased (NHotkey::EHotkey hotkey) const { return m_hotkeysReleased.test(hotkey); }

    // Mouse state
    CFixed GetMouseX () const { return m_mousePosition.x; }
    CFixed GetMouseY () const { return m_mousePosition.y; }
    math::Vec2f GetMousePosition () const { return m_mousePosition; }
    math::Vec2f GetMouseDelta () const { return m_mousePosition - m_lastMousePosition; }

    // Mutators
    void SetHotkeyPressed (NHotkey::EHotkey hotkey) { m_hotkeysPressed.set(hotkey); }
    void SetHotkeyReleased (NHotkey::EHotkey hotkey) { m_hotkeysPressed.set(hotkey, false); m_hotkeysReleased.set(hotkey); }
    void SetMousePosition (math::Vec2f mousePosition) {
        if (m_firstMouseUpdate) {
            m_lastMousePosition = mousePosition;
            m_firstMouseUpdate = false;
        }
        else {
            m_lastMousePosition = m_mousePosition;
        }
        m_mousePosition = mousePosition;
    }

    void Reset () {
        m_lastMousePosition = m_mousePosition;
    }

    // Input processing
    void RegisterHandler (CInputHandler* handler) { m_inputHandlers.push_back(handler); }
    std::vector<CInputHandler*>& GetHandlers () { return m_inputHandlers; }

private:
    // Keyboard state
    std::bitset<NHotkey::Count> m_hotkeysPressed{};
    std::bitset<NHotkey::Count> m_hotkeysReleased{};

    // Mouse state
    bool m_firstMouseUpdate{true};
    math::Vec2f m_lastMousePosition{};
    math::Vec2f m_mousePosition{};

    // Input processing
    std::vector<CInputHandler*> m_inputHandlers{};
};