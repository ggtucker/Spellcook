#pragma once

#include "Math/Vector3.h"

struct SComponentTransform {
    math::Vec3f& Position () { return m_position; }
    const math::Vec3f& Position () const { return m_position; }

    const math::Vec3f& Forward () const { return m_forward; }
    const math::Vec3f& Right () const { return m_right; }
    const math::Vec3f& Up () const { return m_up; }

    math::Vec3f& Scale () { return m_scale; }
    const math::Vec3f& Scale () const { return m_scale; }

    void SetForward (const math::Vec3f& forward, const math::Vec3f& worldUp) {
        m_forward = math::Normalize(forward);
        m_right = math::Normalize(math::Cross(m_forward, worldUp));
        m_up = math::Normalize(math::Cross(m_right, m_forward));
    }

private:
    math::Vec3f m_position{};
    math::Vec3f m_forward{};
    math::Vec3f m_right{};
    math::Vec3f m_up{};
    math::Vec3f m_scale{};
};