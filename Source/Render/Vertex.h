#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"

struct SVertex {
    math::Vec3 m_position{};
    math::Vec3 m_normal{};
    math::Vec2 m_texCoords{};
};