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

    const SSingletonCamera& singleCamera = ReadSingleton<SSingletonCamera>();
    const math::Mat4 view = singleCamera.m_activeCamera.ViewMatrix();
    const math::Mat4 projection = singleCamera.m_activeCamera.ProjectionMatrix();

    for (ecs::SystemEntity entity : SystemIterator()) {
        const SComponentTransform& transform = *entity.ReadComponent<SComponentTransform>();
        const SComponentRender& render = *entity.ReadComponent<SComponentRender>();

        if (CShader* shader = render.m_shader.Deref()) {
            shader->SetMat4("uView", view);
            shader->SetMat4("uProjection", projection);

            math::Mat4 model;
            model = math::Translate(model, transform.Position().Cast<float>());
            model = math::Rotate(model, math::Vec3(1.f, 0.f, 0.f), render.m_timer.TimeElapsed().Seconds());
            shader->SetMat4("uModel", model);

            shader->Use();
        }
        
        NPrimitives::DrawCube();
    }

    g_window.SwapBuffers();
}
