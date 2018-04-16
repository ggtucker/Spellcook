#pragma once

#include "Encosys/EntityId.h"
#include "Render/Camera.h"

struct SSingletonCamera {
    CCamera m_activeCamera;
    ecs::EntityId m_cameraTarget;
};