#include "SystemCamera.h"

#include "Game/ComponentTransform.h"
#include "Math/Matrix4.h"
#include "Render/ComponentCamera.h"
#include "GameUI/SingletonInput.h"

static const CFixed c_sensitivity = F_0_25;

void CSystemCamera::Initialize (ecs::SystemType& type) {
    RequiredComponent<SComponentTransform>(type, ecs::Access::Write);
    RequiredComponent<SComponentCamera>(type, ecs::Access::Write);
    RequiredSingleton<SSingletonInput>(type, ecs::Access::Read);
}

void CSystemCamera::Update (ecs::TimeDelta delta) {
    const SSingletonInput& singleInput = ReadSingleton<SSingletonInput>();

    for (ecs::SystemEntity entity : SystemIterator()) {
        SComponentCamera& camera = *entity.WriteComponent<SComponentCamera>();
        SComponentTransform& transform = *entity.WriteComponent<SComponentTransform>();

        math::Vec2f mouseDelta = singleInput.GetMouseDelta();
        camera.m_yaw -= F_Deg_(mouseDelta.x * c_sensitivity);
        camera.m_pitch += F_Deg_(mouseDelta.y * c_sensitivity);

        if (camera.m_pitch > F_IDeg_(88)) {
            camera.m_pitch = F_IDeg_(88);
        }
        if (camera.m_pitch < -F_IDeg_(88)) {
            camera.m_pitch = -F_IDeg_(88);
        }

        math::Vec3f forward;
        forward.x = math::Cos(camera.m_yaw) * math::Cos(camera.m_pitch);
        forward.y = math::Sin(camera.m_pitch);
        forward.z = math::Sin(camera.m_yaw) * math::Cos(camera.m_pitch);
        transform.SetForward(forward, math::Vec3f(F_0, F_1, F_0));
    }
}
