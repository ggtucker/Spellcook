#pragma once

#include <vector>
#include "Encosys/Encosys.h"

class CInputEvent;
class CInputHandler;
class CWindow;

class CEngine {
public:
    explicit CEngine (CWindow& window) : m_window{window} {}

    void RunMainLoop ();

private:
    void Initialize ();
    void Terminate ();

    void ProcessInput (const CInputEvent& event);
    void Update (CFixed delta);

private:
    CWindow& m_window;
    ecs::Encosys m_encosys;
    std::vector<CInputHandler*> m_inputHandlers{};
};