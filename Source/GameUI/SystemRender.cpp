#include "SystemRender.h"

#include "Math/Matrix4.h"
#include "Render/Primitives.h"

void CSystemRender::PreUpdate () {
    m_window.Clear();
}

void CSystemRender::PostUpdate () {
    m_window.SwapBuffers();
}

void CSystemRender::Update (SystemEntity entity, ecs::TimeDelta delta) {
    const SComponentPosition& position = *entity.ReadComponent<SComponentPosition>();
    const SComponentRender& render = *entity.ReadComponent<SComponentRender>();

    math::Mat4 transform;
    transform = math::Translate(transform, math::Vec3(0.5f, -0.5f, 0.f));
    transform = math::Rotate(transform, math::Vec3(0.f, 0.f, 1.f), render.m_timer.TimeElapsed().Seconds());
    render.m_shader.SetMat4("uTransform", transform);

    render.m_shader.Use();
    NPrimitives::DrawRectangle(position.m_position);
}
