#include "SystemRender.h"

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

    render.m_shader.Use();
    float greenValue = sin(render.m_timer.TimeElapsed().Seconds()) / 2.f + 0.5f;
    render.m_shader.SetVec4f("TriangleColor", CVector4f(F_0, CFixed::FromFloat(greenValue), F_0, F_1));
    NPrimitives::DrawTriangle(position.m_position);
}
