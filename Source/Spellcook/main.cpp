#include "Engine.h"

int main () {
    SWindowContext windowContext;
    windowContext.m_width = 800;
    windowContext.m_height = 600;
    windowContext.m_title = "SPELLCOOK";
    CWindow window(windowContext);

    CEngine engine;
    while (window.IsOpen()) {
        CInputEvent event;
        while (window.PollInput(event)) {
            engine.ProcessInput(window, event);
        }
        engine.Update();
        engine.Render(window);
    }

    return 0;
}