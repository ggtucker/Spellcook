#include "SystemPlayerController.h"

#include "Game/ComponentPlayer.h"
#include "Game/ComponentTransform.h"
#include "GameUI/SingletonInput.h"
#include "Math/Matrix4.h"

static const CFixed c_speed = F_0_5;

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
            transform.Position() -= c_speed * transform.Forward();
        }
        if (singleInput.IsHotkeyPressed(NHotkey::MoveBackward)) {
            transform.Position() += c_speed * transform.Forward();
        }
        if (singleInput.IsHotkeyPressed(NHotkey::MoveRight)) {
            transform.Position() += c_speed * transform.Right();
        }
        if (singleInput.IsHotkeyPressed(NHotkey::MoveLeft)) {
            transform.Position() -= c_speed * transform.Right();
        }
    }
}
