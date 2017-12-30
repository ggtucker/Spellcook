#include "Engine.h"
#include "Render/Window.h"

int main () {
    SWindowContext windowContext;
    windowContext.m_width = 800;
    windowContext.m_height = 600;
    windowContext.m_title = "SPELLCOOK";
    if (!g_window.Create(windowContext)) {
        return -1;
    }
    CEngine engine;
    engine.RunMainLoop();
    return 0;
}