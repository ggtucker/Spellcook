#pragma once

#include "Encosys/System.h"

class CWindow;

struct CSystemRender : public ecs::System {
    virtual void Initialize (ecs::SystemType& type) override;
    virtual void Update (ecs::SystemContext& context, ecs::TimeDelta delta) override;
};