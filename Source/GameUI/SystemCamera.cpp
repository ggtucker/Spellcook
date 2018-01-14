#include "SystemCamera.h"

#include "Game/ComponentTransform.h"
#include "GameUI/SingletonCamera.h"

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
            const math::Vec3f& position = transform->Position();
            const math::Vec3f& forward = transform->Forward();
            camera.SetPosition(math::Vec3(position.x.ToFloat(), position.y.ToFloat(), position.z.ToFloat()));
            camera.SetForward(math::Vec3(forward.x.ToFloat(), forward.y.ToFloat(), forward.z.ToFloat()));
        }
    }
}
