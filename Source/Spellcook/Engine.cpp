#include "Engine.h"

#include "Render/Primitives.h"
#include "Render/Window.h"

void CEngine::Initialize () {
    m_shader.Create("..\\Bin\\Shaders\\DefaultShader.vs", "..\\Bin\\Shaders\\DefaultShader.fs");
}

void CEngine::Terminate () {

}

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

    m_shader.Use();
    float greenValue = sin(m_colorTimer.TimeElapsed().Seconds()) / 2.f + 0.5f;
    m_shader.SetVec4f("TriangleColor", CVector4f(F_0, CFixed::FromFloat(greenValue), F_0, F_1));
    NPrimitives::DrawTriangle(CVector3f());

    window.Display();
}