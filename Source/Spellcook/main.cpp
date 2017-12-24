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

    CTimer engineTimer;
    CTime accumulatedTime;
    const CFixed c_fixedTimestep = F_0_03125;
    const CTime c_timestep = CTime::Seconds(c_fixedTimestep.ToFloat());

    CEngine engine;
    engine.Initialize(window);
    while (window.IsOpen()) {
        CInputEvent event;
        while (window.PollInput(event)) {
            engine.ProcessInput(window, event);
        }

        CTime frameTime = engineTimer.Restart();
        accumulatedTime += frameTime;

        while (accumulatedTime >= c_timestep) {
            engine.Update(c_fixedTimestep);
            accumulatedTime -= c_timestep;
        }
    }
    engine.Terminate();

    return 0;
}