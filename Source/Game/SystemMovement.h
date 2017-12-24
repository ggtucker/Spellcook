#pragma once

#include "Encosys/System.h"
#include "ComponentPosition.h"
#include "ComponentVelocity.h"

struct CSystemMovement : public ecs::ParallelSystem<
    ecs::ComponentDependency<SComponentPosition, ecs::Existence::Required, ecs::Access::Write>,
    ecs::ComponentDependency<SComponentVelocity, ecs::Existence::Required, ecs::Access::Read>
> {
    virtual void Update (SystemEntity entity, ecs::TimeDelta delta) override {
        const SComponentVelocity& velocity = *entity.ReadComponent<SComponentVelocity>();
        SComponentPosition& position = *entity.WriteComponent<SComponentPosition>();
        position.m_position += velocity.m_velocity * delta;
    }
};