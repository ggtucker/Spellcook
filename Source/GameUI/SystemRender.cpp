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

    math::Mat4 model;
    model = math::Rotate(model, math::Vec3(1.f, 0.f, 0.f), render.m_timer.TimeElapsed().Seconds());
    render.m_shader.SetMat4("uModel", model);

    math::Mat4 view;
    view = math::Translate(view, math::Vec3(0.f, 0.f, -3.f));
    render.m_shader.SetMat4("uView", view);

    math::Mat4 projection;
    projection = math::Perspective(0.78539f, 800.f / 600.f, 0.1f, 100.f);
    render.m_shader.SetMat4("uProjection", projection);

    render.m_shader.Use();
    NPrimitives::DrawCube(position.m_position);
}
