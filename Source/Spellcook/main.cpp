#include "Encosys/Encosys.h"
#include "Engine.h"
#include "Game/SystemMovement.h"
#include "GameUI/SystemRender.h"
#include "Render/Timer.h"

int main () {
    CWindow window;
    SWindowContext windowContext;
    windowContext.m_width = 800;
    windowContext.m_height = 600;
    windowContext.m_title = "SPELLCOOK";
    if (!window.Create(windowContext)) {
        return -1;
    }

    CEngine engine(window);
    engine.RunMainLoop();

    return 0;
}