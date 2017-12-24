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
    explicit CEngine (CWindow& window) : m_window{window} {}

    void RunMainLoop ();

private:
    void Initialize();
    void Terminate();

    void ProcessInput(const CInputEvent& event);
    void Update(CFixed delta);

private:
    CWindow& m_window;
    ecs::Encosys m_encosys;
    std::vector<CInputHandler*> m_inputHandlers{};
};