#include "InputHandler.h"

bool CInputHandler::ProcessInput (CWindow& window, const CInputEvent& input) {
    switch (input.m_type) {
        case EInputEvent::Resized:       return OnResize(window, input.m_resize);
        case EInputEvent::KeyPressed:    return OnKeyPressed(window, input.m_key);
        case EInputEvent::KeyReleased:   return OnKeyReleased(window, input.m_key);
        case EInputEvent::MouseScrolled: return OnMouseScrolled(window, input.m_mouseScroll);
        case EInputEvent::MousePressed:  return OnMousePressed(window, input.m_mouseButton);
        case EInputEvent::MouseReleased: return OnMouseReleased(window, input.m_mouseButton);
        case EInputEvent::MouseMoved:    return OnMouseMoved(window, input.m_mouseMove);
        case EInputEvent::MouseEntered:  return OnMouseEntered(window);
        case EInputEvent::MouseLeft:     return OnMouseLeft(window);
    }
    return false;
}
