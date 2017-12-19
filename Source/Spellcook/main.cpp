#include "Encosys/EntityManager.h"
#include "Math/Vector2.h"
#include "Render/InputSystem.h"
#include "Render/Window.h"
#include <iostream>
#include <thread>

static void RenderThread (CWindow& window) {
    window.SetActive(true);
    while (window.IsOpen()) {
        window.Clear();
        // draw shit
        window.Display();
    }
}

int main () {
    ecs::EntityManager entities;
    entities.CreateEntity();

    SWindowContext windowContext;
    windowContext.m_width = 800;
    windowContext.m_height = 600;
    windowContext.m_title = "SPELLCOOK";
    CWindow window(windowContext);

    //window.SetActive(false);
    //std::thread renderThread(RenderThread, std::ref(window));

    CFixed a = F_4;
    CVector2f vector(F_1, F_2);
    vector.Normalize();
    std::cout << "Result: " << F_2 << std::endl;
    std::cout << "Result: " << vector.Mag() << std::endl;
    std::cout << "Result: " << a.Squared() << std::endl;

    CInputSystem inputSystem;
    while (window.IsOpen()) {
        CInputEvent input;
        while (window.PollInput(input)) {
            inputSystem.ProcessInput(input);
        }
        window.Clear();
        // draw shit
        window.Display();
    }

    //renderThread.join();
    return 0;
}