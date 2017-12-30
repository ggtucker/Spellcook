#pragma once

#include "Encosys/System.h"
#include "Game/ComponentPosition.h"
#include "Render/ComponentRender.h"
#include "Render/Window.h"

struct CSystemRender : public ecs::ParallelSystem<
    ecs::ComponentDependency<SComponentPosition, ecs::Existence::Required, ecs::Access::Read>,
    ecs::ComponentDependency<SComponentRender, ecs::Existence::Required, ecs::Access::Read>
> {
    explicit CSystemRender (CWindow& window) : m_window{window} {}

    virtual void PreUpdate () override;
    virtual void PostUpdate () override;
    virtual void Update (SystemEntity entity, ecs::TimeDelta delta) override;

private:
    CWindow& m_window;
};