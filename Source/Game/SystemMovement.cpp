#include "SystemMovement.h"

#include "Game/ComponentPosition.h"
#include "Game/ComponentVelocity.h"

void CSystemMovement::Initialize (ecs::SystemType& type) {
    RequiredComponent<SComponentPosition>(type, ecs::ComponentUsage::Write);
    RequiredComponent<SComponentVelocity>(type, ecs::ComponentUsage::Read);
}

void CSystemMovement::Update (ecs::SystemContext& context, ecs::TimeDelta delta) {
    for (uint32_t i = 0; i < context.EntityCount(); ++i) {
        const SComponentVelocity& velocity = *context.ReadComponent<SComponentVelocity>(i);
        SComponentPosition& position = *context.WriteComponent<SComponentPosition>(i);
        position.m_position += velocity.m_velocity * CFixed::FromFloat(delta);
    }
}
