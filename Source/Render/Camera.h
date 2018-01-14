#pragma once

#include "Math/Matrix4.h"

class CCamera {
public:
    math::Mat4 ViewMatrix () const;
    math::Mat4 ProjectionMatrix () const;

    void SetPosition (const math::Vec3& position) { m_position = position; }
    void SetForward (const math::Vec3& forward) { m_forward = forward; }

private:
    math::Vec3 m_position{};
    math::Vec3 m_forward{};
    math::Vec3 m_worldUp{0.f, 1.f, 0.f};
    float m_aspect{4.f / 3.f};
    float m_fov{0.78539f}; // quarter pi
    float m_near{0.1f};
    float m_far{100.f};
};