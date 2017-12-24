#include "Engine.h"
#include <iostream>

int main () {
    CWindow window;
    SWindowContext windowContext;
    windowContext.m_width = 800;
    windowContext.m_height = 600;
    windowContext.m_title = "SPELLCOOK";
    if (!window.Create(windowContext)) {
        return -1;
    }

    CEngine engine;
    engine.Initialize();
    while (window.IsOpen()) {
        CInputEvent event;
        while (window.PollInput(event)) {
            engine.ProcessInput(window, event);
        }
        engine.Update();
        engine.Render(window);
    }
    engine.Terminate();

    return 0;
}