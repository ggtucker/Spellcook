#pragma once

#include <vector>
#include "GameUI/InputHandler.h"
#include "Math/Vector2.h"
#include "Render/Shader.h"
#include "Render/Timer.h"

class CWindow;

class CEngine {
public:

    void Initialize ();
    void Terminate ();

    void ProcessInput (CWindow& window, const CInputEvent& event);

    void Update ();

    void Render (CWindow& window);

private:
    CTimer m_colorTimer{};
    CVector2f m_pos{};
    std::vector<CInputHandler*> m_inputHandlers{};
    CShader m_shader{};
};