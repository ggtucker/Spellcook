#pragma once

#include "Math/Vector3.h"

struct SComponentTransform {
    math::Vec3& Position () { return m_position; }
    const math::Vec3& Position () const { return m_position; }

    const math::Vec3& Forward () const { return m_forward; }
    const math::Vec3& Right () const { return m_right; }
    const math::Vec3& Up () const { return m_up; }

    math::Vec3& Scale () { return m_scale; }
    const math::Vec3& Scale () const { return m_scale; }

    void SetForward (const math::Vec3& forward, const math::Vec3& worldUp) {
        m_forward = math::Normalize(forward);
        m_right = math::Normalize(math::Cross(m_forward, worldUp));
        m_up = math::Normalize(math::Cross(m_right, m_forward));
    }

    float GetYaw () const { return m_yaw; }
    float GetPitch () const { return m_pitch; }

    void SetYaw (float yaw) { m_yaw = yaw; }
    void SetPitch (float pitch) { m_pitch = pitch; }

private:
    math::Vec3 m_position{};
    math::Vec3 m_forward{};
    math::Vec3 m_right{};
    math::Vec3 m_up{};
    math::Vec3 m_scale{};

    // TODO: replace with quaternion
    float m_yaw{-FA_Deg_(90)};
    float m_pitch{};
};