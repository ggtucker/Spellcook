#include "Engine.h"
#include "Render/Window.h"

#include "Core/Fixed.h"
#include "Math/Trig.h"
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

    std::cout << "S(    0):" << math::Sin(F_0) << " C(    0):" << math::Cos(F_0) << std::endl;
    std::cout << "S( pi/2):" << math::Sin(F_1) << " C( pi/2):" << math::Cos(F_1) << std::endl;
    std::cout << "S(   pi):" << math::Sin(F_2) << " C(   pi):" << math::Cos(F_2) << std::endl;
    std::cout << "S(3pi/2):" << math::Sin(F_3) << " C(3pi/2):" << math::Cos(F_3) << std::endl;
    std::cout << "S(  2pi):" << math::Sin(F_4) << " C(  2pi):" << math::Cos(F_4) << std::endl;

    CEngine engine(window);
    engine.RunMainLoop();

    return 0;
}