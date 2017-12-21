#include "Engine.h"

#include "Render/Primitives.h"
#include "Render/Window.h"

void CEngine::ProcessInput (CWindow& window, const CInputEvent& event) {
    if (event.m_type == EInputEvent::Closed) {
        window.Close();
    }
    else {
        for (CInputHandler* inputHandler : m_inputHandlers) {
            if (inputHandler->ProcessInput(window, event)) {
                return;
            }
        }
    }
}

void CEngine::Update () {

}

void CEngine::Render (CWindow& window) {
    window.Clear();

    NPrimitives::DrawTriangle(CVector3f());

    window.Display();
}