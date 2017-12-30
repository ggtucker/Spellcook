#pragma once

#include "Encosys.h"
#include "SystemType.h"

namespace ecs {

class SystemEntity {
public:
    SystemEntity (Encosys& encosys, const SystemType& systemType, Entity entity) :
        m_encosys{encosys},
        m_systemType{systemType},
        m_entity{entity} {
    }

    template <typename TComponent>
    TComponent* WriteComponent () {
        ENCOSYS_ASSERT_(m_systemType.IsWriteAllowed(m_encosys.GetComponentTypeId<TComponent>()));
        return m_entity.GetComponent<TComponent>();
    }

    template <typename TComponent>
    const TComponent* ReadComponent () const {
        ENCOSYS_ASSERT_(m_systemType.IsReadAllowed(m_encosys.GetComponentTypeId<TComponent>()));
        return m_entity.GetComponent<TComponent>();
    }

private:
    Encosys& m_encosys;
    const SystemType& m_systemType;
    Entity m_entity;
};

class SystemIterType {
public:
    SystemIterType (Encosys& encosys, const SystemType& systemType, uint32_t index) :
        m_encosys{encosys},
        m_systemType{systemType},
        m_index{index} {
        Next();
    }

    bool operator== (SystemIterType rhs) { return m_index == rhs.m_index; }
    bool operator!= (SystemIterType rhs) { return m_index != rhs.m_index; }
    SystemEntity operator* () { return SystemEntity(m_encosys, m_systemType, m_encosys[m_index]); }
    void operator++ () { ++m_index; Next(); }

private:

    void Next () {
        while (m_index < m_encosys.ActiveEntityCount()) {
            Entity entity = m_encosys[m_index];
            if (entity.HasComponentBitset(m_systemType.GetRequiredBitset())) {
                break;
            }
            ++m_index;
        }
    }

    Encosys& m_encosys;
    const SystemType& m_systemType;
    uint32_t m_index;
};

class SystemIter {
public:
    explicit SystemIter (Encosys& encosys, SystemTypeId systemId) : m_encosys{encosys}, m_systemType{encosys.GetSystemType(systemId)} {}

    SystemIterType begin () { return SystemIterType(m_encosys, m_systemType, 0); }
    SystemIterType end () { return SystemIterType(m_encosys, m_systemType, m_encosys.ActiveEntityCount()); }

private:
    Encosys& m_encosys;
    const SystemType& m_systemType;
};

}