#include "Encosys/EntityManager.h"
#include "Math/Vector2.h"
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

    CFixed a = F_4;
    CVector2f vector(F_1, F_2);
    vector.Normalize();
    std::cout << "Result: " << F_2 << std::endl;
    std::cout << "Result: " << vector.Mag() << std::endl;
    std::cout << "Result: " << a.Squared() << std::endl;

    CInputSystem input;
    while (window.IsOpen()) {
        window.ProcessInput(input);
        window.Clear();
        // draw shit
        window.Display();
    }

    return 0;
}