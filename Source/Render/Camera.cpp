#include "Camera.h"

math::Mat4 CCamera::ViewMatrix () const {
    return math::LookAt(m_position, m_position + m_forward, m_worldUp);
}

math::Mat4 CCamera::ProjectionMatrix () const {
    return math::Perspective(m_fov, m_aspect, m_near, m_far);
}