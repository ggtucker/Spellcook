#pragma once

#include "Encosys/System.h"

class CWindow;

struct CSystemRender : public ecs::System {
    explicit CSystemRender (CWindow& window) : m_window{window} {}

    virtual void Initialize (ecs::Encosys& encosys, ecs::SystemType& type) override;
    virtual void Update (ecs::SystemContext& context, ecs::TimeDelta delta) override;

private:
    CWindow& m_window;
};