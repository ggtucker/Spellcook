#include "Encosys/EntityManager.h"
#include "Render/InputSystem.h"
#include "Render/Window.h"
#include <iostream>

int main () {
    ecs::EntityManager entities;
    entities.CreateEntity();

    CWindow window;
    SWindowContext windowContext;
    windowContext.m_width = 800;
    windowContext.m_height = 600;
    windowContext.m_title = "SPELLCOOK";
    window.Create(windowContext);

    CFixed value = 4;
    std::cout << value.Sqrt() << std::endl;

    CInputSystem input;
    while (window.IsOpen()) {
        window.ProcessInput(input);
        window.Clear();
        // draw shit
        window.Display();
    }

    return 0;
}