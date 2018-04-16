#pragma once

#include "Render/Window.h"
#include "Render/InputEvent.h"

class CInputHandler {
public:
    bool ProcessInput (CWindow& window, const CInputEvent& event);

private:
    virtual bool OnResize (CWindow& window, const CInputEvent::SResizeEvent& event) { return false; }
    virtual bool OnKeyPressed (CWindow& window, const CInputEvent::SKeyEvent& event) { return false; }
    virtual bool OnKeyReleased (CWindow& window, const CInputEvent::SKeyEvent& event) { return false; }
    virtual bool OnMouseScrolled (CWindow& window, const CInputEvent::SMouseScrollEvent& event) { return false; }
    virtual bool OnMousePressed (CWindow& window, const CInputEvent::SMouseButtonEvent& event) { return false; }
    virtual bool OnMouseReleased (CWindow& window, const CInputEvent::SMouseButtonEvent& event) { return false; }
    virtual bool OnMouseMoved (CWindow& window, const CInputEvent::SMouseMoveEvent& event) { return false; }
    virtual bool OnMouseEntered (CWindow& window) { return false; }
    virtual bool OnMouseLeft (CWindow& window) { return false; }
};
