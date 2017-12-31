#include "SystemMovement.h"

#include "Game/ComponentTransform.h"
#include "Game/ComponentVelocity.h"

void CSystemMovement::Initialize (ecs::SystemType& type) {
    RequiredComponent<SComponentTransform>(type, ecs::Access::Write);
    RequiredComponent<SComponentVelocity>(type, ecs::Access::Read);
}

void CSystemMovement::Update (ecs::TimeDelta delta) {
    for (ecs::SystemEntity entity : SystemIterator()) {
        const SComponentVelocity& velocity = *entity.ReadComponent<SComponentVelocity>();
        SComponentTransform& transform = *entity.WriteComponent<SComponentTransform>();
        transform.Position() += velocity.m_velocity * delta;
    }
}
