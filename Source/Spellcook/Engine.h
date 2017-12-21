#pragma once

#include "GameUI/InputHandler.h"
#include <vector>
#include "Math/Vector2.h"

class CWindow;

class CEngine {
public:

    void ProcessInput (CWindow& window, const CInputEvent& event);

    void Update ();

    void Render (CWindow& window);

private:
    CVector2f m_pos{};
    std::vector<CInputHandler*> m_inputHandlers{};
};