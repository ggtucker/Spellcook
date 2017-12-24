#pragma once

#include <vector>
#include "GameUI/InputHandler.h"
#include "Math/Vector2.h"
#include "Render/Shader.h"
#include "Render/Timer.h"
#include "Encosys/Encosys.h"

class CWindow;

class CEngine {
public:
    void Initialize (CWindow& window);
    void Terminate ();

    void ProcessInput (CWindow& window, const CInputEvent& event);

    void Update (CFixed delta);

private:
    ecs::Encosys m_encosys;
    std::vector<CInputHandler*> m_inputHandlers{};
};