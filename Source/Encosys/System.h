#pragma once

#include <vector>
#include "Encosys.h"
#include "SystemType.h"

namespace ecs {

class SystemRegistry;

class System {
public:
    virtual ~System () = default;

    virtual void Initialize (SystemType& type) = 0;
    virtual void Update (SystemContext& context, TimeDelta delta) = 0;

protected:
    template <typename TComponent> void RequiredComponent (SystemType& type, ComponentUsage usage) {
        type.RequiredComponent(GetEncosys().GetComponentType<TComponent>(), usage);
    }

    template <typename TComponent> void OptionalComponent (SystemType& type, ComponentUsage usage) {
        type.OptionalComponent(GetEncosys().GetComponentType<TComponent>(), usage);
    }

    Encosys& GetEncosys () { return *m_encosys; }
    SystemTypeId GetTypeId () { return m_typeId; }

private:
    friend class SystemRegistry;
    Encosys* m_encosys;
    SystemTypeId m_typeId;
};

}