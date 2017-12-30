#include "SystemRender.h"

#include "Game/ComponentPosition.h"
#include "Math/Matrix4.h"
#include "Render/ComponentRender.h"
#include "Render/Primitives.h"
#include "Render/Window.h"

void CSystemRender::Initialize (ecs::SystemType& type) {
    RequiredComponent<SComponentPosition>(type, ecs::ComponentUsage::Read);
    RequiredComponent<SComponentRender>(type, ecs::ComponentUsage::Read);
}

void CSystemRender::Update (ecs::SystemContext& context, ecs::TimeDelta delta) {
    g_window.Clear();

    for (uint32_t i = 0; i < context.EntityCount(); ++i) {
        const SComponentPosition& position = *context.ReadComponent<SComponentPosition>(i);
        const SComponentRender& render = *context.ReadComponent<SComponentRender>(i);

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

    g_window.SwapBuffers();
}
