#pragma once

#include <vector>
#include "Core/Fixed.h"
#include "Encosys/Encosys.h"

class CInputEvent;
class CInputHandler;

class CEngine {
public:
    void RunMainLoop ();

private:
    void Initialize ();
    void Terminate ();

    void Update (CFixed delta);

private:
    ecs::Encosys m_encosys;
};