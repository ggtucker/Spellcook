#pragma once

#include "Encosys.h"
#include "SystemIter.h"
#include "SystemType.h"

namespace ecs {

class SystemRegistry;

class System {
public:
    virtual ~System () = default;

    virtual void Initialize (SystemType& type) = 0;
    virtual void Update (TimeDelta delta) = 0;

protected:
    template <typename TComponent> void RequiredComponent (SystemType& type, ComponentUsage usage) {
        type.RequiredComponent(GetEncosys().GetComponentTypeId<TComponent>(), usage);
    }

    template <typename TComponent> void OptionalComponent (SystemType& type, ComponentUsage usage) {
        type.OptionalComponent(GetEncosys().GetComponentTypeId<TComponent>(), usage);
    }

    Encosys& GetEncosys () { return *m_encosys; }
    SystemIter SystemIterator () { return SystemIter(*m_encosys, m_typeId); }

private:
    friend class SystemRegistry;
    Encosys* m_encosys;
    SystemTypeId m_typeId;
};

}