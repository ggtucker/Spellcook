#pragma once

#include "Core/Fixed.h"
#include "Encosys/EntityId.h"

struct SComponentCamera {
    ecs::EntityId m_followTarget;
    CFixed m_distance;

    CFixed m_yaw{-F_IDeg_(90)};
    CFixed m_pitch;
};