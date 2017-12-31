#include "SystemRender.h"

#include "Game/ComponentTransform.h"
#include "Math/Matrix4.h"
#include "Render/ComponentRender.h"
#include "Render/SingletonCamera.h"
#include "Render/Primitives.h"
#include "Render/Window.h"

void CSystemRender::Initialize (ecs::SystemType& type) {
    RequiredComponent<SComponentTransform>(type, ecs::Access::Read);
    RequiredComponent<SComponentRender>(type, ecs::Access::Read);
    RequiredSingleton<SSingletonCamera>(type, ecs::Access::Read);
}

void CSystemRender::Update (ecs::TimeDelta delta) {
    g_window.Clear();

    math::Mat4 view;
    const SSingletonCamera& singleCamera = ReadSingleton<SSingletonCamera>();
    ecs::SystemEntity camera = GetEntity(singleCamera.m_activeCamera);
    if (camera.IsValid()) {
        const SComponentTransform& cameraTransform = *camera.ReadComponent<SComponentTransform>();
        view = math::LookAt(
            cameraTransform.Position(),
            cameraTransform.Position() + cameraTransform.Forward(),
            cameraTransform.Up()
        ).Cast<float>();
    }

    for (ecs::SystemEntity entity : SystemIterator()) {
        const SComponentTransform& transform = *entity.ReadComponent<SComponentTransform>();
        const SComponentRender& render = *entity.ReadComponent<SComponentRender>();

        math::Mat4 model;
        model = math::Rotate(model, math::Vec3(1.f, 0.f, 0.f), render.m_timer.TimeElapsed().Seconds());
        render.m_shader.SetMat4("uModel", model);

        render.m_shader.SetMat4("uView", view);

        math::Mat4 projection;
        projection = math::Perspective(0.78539f, 800.f / 600.f, 0.1f, 100.f);
        render.m_shader.SetMat4("uProjection", projection);

        render.m_shader.Use();
        NPrimitives::DrawCube(transform.Position());
    }

    g_window.SwapBuffers();
}
