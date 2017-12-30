#pragma once

#include "Encosys/System.h"

struct CSystemMovement : public ecs::System {
    virtual void Initialize (ecs::SystemType& type) override;
    virtual void Update (ecs::SystemContext& context, ecs::TimeDelta delta) override;
};