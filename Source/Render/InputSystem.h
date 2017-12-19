#pragma once

#include "Core/CoreTypes.h"
#include <functional>
#include "InputEvent.h"

class CInputListener {

};

class CInputSystem {
public:
    // Logical Event Registration


    // OS Input Event Handlers
    void ProcessInput (const CInputEvent& event);

private:
};