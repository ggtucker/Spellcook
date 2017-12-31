#include "SystemInput.h"

#include "SingletonInput.h"
#include "GameUI/InputHandler.h"
#include "Render/SingletonCamera.h"
#include "Render/Window.h"

void OnKeyPressed (SSingletonInput& input, const CInputEvent::SKeyEvent& event) {
    if (event.m_key == NKeyboard::W) {
        input.SetHotkeyPressed(NHotkey::MoveForward);
    }
    else if (event.m_key == NKeyboard::S) {
        input.SetHotkeyPressed(NHotkey::MoveBackward);
    }
    else if (event.m_key == NKeyboard::A) {
        input.SetHotkeyPressed(NHotkey::MoveLeft);
    }
    else if (event.m_key == NKeyboard::D) {
        input.SetHotkeyPressed(NHotkey::MoveRight);
    }
}

void OnKeyReleased (SSingletonInput& input, const CInputEvent::SKeyEvent& event) {
    if (event.m_key == NKeyboard::W) {
        input.SetHotkeyReleased(NHotkey::MoveForward);
    }
    else if (event.m_key == NKeyboard::S) {
        input.SetHotkeyReleased(NHotkey::MoveBackward);
    }
    else if (event.m_key == NKeyboard::A) {
        input.SetHotkeyReleased(NHotkey::MoveLeft);
    }
    else if (event.m_key == NKeyboard::D) {
        input.SetHotkeyReleased(NHotkey::MoveRight);
    }
}

void OnMouseScrolled (SSingletonInput& input, const CInputEvent::SMouseScrollEvent& event) {

}

void OnMousePressed (SSingletonInput& input, const CInputEvent::SMouseButtonEvent& event) {

}

void OnMouseReleased (SSingletonInput& input, const CInputEvent::SMouseButtonEvent& event) {

}

void OnMouseMoved (SSingletonInput& input, const CInputEvent::SMouseMoveEvent& event) {
    input.SetMousePosition({event.m_x, event.m_y});
}

void OnMouseEntered (SSingletonInput& input) {

}

void OnMouseLeft (SSingletonInput& input) {

}

void CSystemInput::Initialize (ecs::SystemType& type) {
    RequiredSingleton<SSingletonInput>(type, ecs::Access::Write);
    RequiredSingleton<SSingletonCamera>(type, ecs::Access::Read);
}

void CSystemInput::Update (ecs::TimeDelta delta) {
    SSingletonInput& singleInput = WriteSingleton<SSingletonInput>();
    singleInput.Reset();

    CInputEvent event;
    while (g_window.PollInput(event)) {
        if (event.m_type == EInputEvent::Closed) {
            g_window.Close();
        }
        else {
            for (CInputHandler* inputHandler : singleInput.GetHandlers()) {
                if (inputHandler->ProcessInput(g_window, event)) {
                    return;
                }
            }
            // If no input handler consumed the input let the game sim have it
            switch (event.m_type) {
                case EInputEvent::KeyPressed:    OnKeyPressed(singleInput, event.m_key);            break;
                case EInputEvent::KeyReleased:   OnKeyReleased(singleInput, event.m_key);           break;
                case EInputEvent::MouseScrolled: OnMouseScrolled(singleInput, event.m_mouseScroll); break;
                case EInputEvent::MousePressed:  OnMousePressed(singleInput, event.m_mouseButton);  break;
                case EInputEvent::MouseReleased: OnMouseReleased(singleInput, event.m_mouseButton); break;
                case EInputEvent::MouseMoved:    OnMouseMoved(singleInput, event.m_mouseMove);      break;
                case EInputEvent::MouseEntered:  OnMouseEntered(singleInput);                       break;
                case EInputEvent::MouseLeft:     OnMouseLeft(singleInput);                          break;
            }
        }
    }
}
