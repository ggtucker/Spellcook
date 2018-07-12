#include "SystemPlayerController.h"

#include "Game/ComponentPlayer.h"
#include "Game/ComponentTransform.h"
#include "GameRender/SingletonInput.h"
#include "Math/Matrix4.h"

static const float c_movementSpeed = 0.5f;
static const float c_turnSensitivity = 0.25f;
static constexpr float c_maxPitchRadians = math::DegreesToRadians(88);

void CSystemPlayerController::Initialize (ecs::SystemType& type) {
    RequiredComponent<SComponentPlayer>(type, ecs::Access::Read);
    RequiredComponent<SComponentTransform>(type, ecs::Access::Write);
    RequiredSingleton<SSingletonInput>(type, ecs::Access::Read);
}

void CSystemPlayerController::Update (ecs::TimeDelta delta) {
    const SSingletonInput& singleInput = ReadSingleton<SSingletonInput>();
    for (ecs::SystemEntity entity : SystemIterator()) {
        SComponentTransform& transform = *entity.WriteComponent<SComponentTransform>();
        if (singleInput.IsHotkeyPressed(NHotkey::MoveForward)) {
            transform.Position() -= c_movementSpeed * transform.Forward();
        }
        if (singleInput.IsHotkeyPressed(NHotkey::MoveBackward)) {
            transform.Position() += c_movementSpeed * transform.Forward();
        }
        if (singleInput.IsHotkeyPressed(NHotkey::MoveRight)) {
            transform.Position() += c_movementSpeed * transform.Right();
        }
        if (singleInput.IsHotkeyPressed(NHotkey::MoveLeft)) {
            transform.Position() -= c_movementSpeed * transform.Right();
        }

        math::Vec2 mouseDelta = singleInput.GetMouseDelta();
        float yaw = transform.GetYaw() - math::DegreesToRadians(mouseDelta.x * c_turnSensitivity);
        float pitch = transform.GetPitch() + math::DegreesToRadians(mouseDelta.y * c_turnSensitivity);

        if (pitch > c_maxPitchRadians) {
            pitch = c_maxPitchRadians;
        }
        else if (pitch < -c_maxPitchRadians) {
            pitch = -c_maxPitchRadians;
        }

        math::Vec3 forward;
        forward.x = math::Cos(yaw) * math::Cos(pitch);
        forward.y = math::Sin(pitch);
        forward.z = math::Sin(yaw) * math::Cos(pitch);
        transform.SetForward(forward, math::Vec3(0.f, 1.f, 0.f));
        transform.SetYaw(yaw);
        transform.SetPitch(pitch);
    }
}
