#include "SystemRender.h"

#include "Game/ComponentTransform.h"
#include "Math/Matrix4.h"
#include "GameUI/ComponentRender.h"
#include "GameUI/SingletonCamera.h"
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
            cameraTransform.Position().Cast<float>(),
            cameraTransform.Position().Cast<float>() + cameraTransform.Forward().Cast<float>(),
            math::Vec3(0.f, 1.f, 0.f)
        );
    }

    for (ecs::SystemEntity entity : SystemIterator()) {
        const SComponentTransform& transform = *entity.ReadComponent<SComponentTransform>();
        const SComponentRender& render = *entity.ReadComponent<SComponentRender>();

        if (CShader* shader = render.m_shader.Deref()) {
            math::Mat4 model;
            model = math::Translate(model, transform.Position().Cast<float>());
            model = math::Rotate(model, math::Vec3(1.f, 0.f, 0.f), render.m_timer.TimeElapsed().Seconds());
            shader->SetMat4("uModel", model);

            shader->SetMat4("uView", view);

            math::Mat4 projection;
            projection = math::Perspective(0.78539f, 800.f / 600.f, 0.1f, 100.f);
            shader->SetMat4("uProjection", projection);

            shader->Use();
        }
        
        NPrimitives::DrawCube();
    }

    g_window.SwapBuffers();
}
