#include "SystemCamera.h"

#include "Game/ComponentTransform.h"
#include "GameRender/SingletonCamera.h"

void CSystemCamera::Initialize (ecs::SystemType& type) {
    OptionalComponent<SComponentTransform>(type, ecs::Access::Read);
    RequiredSingleton<SSingletonCamera>(type, ecs::Access::Write);
}

void CSystemCamera::Update (ecs::TimeDelta delta) {
    SSingletonCamera& singleCamera = WriteSingleton<SSingletonCamera>();

    CCamera& camera = singleCamera.m_activeCamera;
    ecs::SystemEntity cameraTarget = GetEntity(singleCamera.m_cameraTarget);

    if (cameraTarget.IsValid()) {
        if (const SComponentTransform* transform = cameraTarget.ReadComponent<SComponentTransform>()) {
            const math::Vec3& position = transform->Position();
            const math::Vec3& forward = transform->Forward();
            camera.SetPosition(position);
            camera.SetForward(forward);
        }
    }
}
