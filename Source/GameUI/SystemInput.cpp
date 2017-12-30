#include "SystemInput.h"

#include "SingletonInput.h"

void CSystemInput::Initialize (ecs::SystemType& type) {
    RequiredSingleton<SSingletonInput>(type, ecs::Access::Read);
}

void CSystemInput::Update (ecs::TimeDelta delta) {
    const SSingletonInput& input = ReadSingleton<SSingletonInput>();

}
