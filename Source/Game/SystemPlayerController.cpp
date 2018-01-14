#include "SystemPlayerController.h"

#include "Game/ComponentPlayer.h"
#include "Game/ComponentTransform.h"
#include "GameUI/SingletonInput.h"
#include "Math/Matrix4.h"

static const CFixed c_movementSpeed = F_0_5;
static const CFixed c_turnSensitivity = F_0_25;

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

        math::Vec2f mouseDelta = singleInput.GetMouseDelta();
        CFixed yaw = transform.GetYaw() - FA_Deg_(mouseDelta.x * c_turnSensitivity);
        CFixed pitch = transform.GetPitch() + FA_Deg_(mouseDelta.y * c_turnSensitivity);

        if (pitch > FA_Deg_(88)) {
            pitch = FA_Deg_(88);
        }
        else if (pitch < -FA_Deg_(88)) {
            pitch = -FA_Deg_(88);
        }

        math::Vec3f forward;
        forward.x = math::Cos(yaw) * math::Cos(pitch);
        forward.y = math::Sin(pitch);
        forward.z = math::Sin(yaw) * math::Cos(pitch);
        transform.SetForward(forward, math::Vec3f(F_0, F_1, F_0));
        transform.SetYaw(yaw);
        transform.SetPitch(pitch);
    }
}
