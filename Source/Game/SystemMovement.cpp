#include "SystemMovement.h"

#include "Game/ComponentPosition.h"
#include "Game/ComponentVelocity.h"

void CSystemMovement::Initialize (ecs::SystemType& type) {
    RequiredComponent<SComponentPosition>(type, ecs::ComponentUsage::Write);
    RequiredComponent<SComponentVelocity>(type, ecs::ComponentUsage::Read);
}

void CSystemMovement::Update (ecs::TimeDelta delta) {
    for (ecs::SystemEntity entity : SystemIterator()) {
        const SComponentVelocity& velocity = *entity.ReadComponent<SComponentVelocity>();
        SComponentPosition& position = *entity.WriteComponent<SComponentPosition>();
        position.m_position += velocity.m_velocity * CFixed::FromFloat(delta);
    }
}
