#pragma once

#include "BlockObjectPool.h"
#include "EncosysTypes.h"
#include "SystemType.h"
#include <array>
#include <cassert>
#include <map>
#include <typeindex>

namespace ecs {

class System;

class SystemRegistry {
public:
    virtual ~SystemRegistry ();

    template <typename TSystem, typename... TArgs>
    SystemTypeId Register (TArgs&&... args) {
        static_assert(std::is_base_of<System, TSystem>::value, "TSystem must be derived from System");
        using TDecayed = std::decay_t<TSystem>;
        const SystemTypeId id = Count();
        assert(m_typeToId.find(typeid(TDecayed)) == m_typeToId.end());
        m_systemTypes.push_back(SystemType(id));
        m_typeToId[typeid(TDecayed)] = id;

        System* system = new TSystem(std::forward<TArgs>(args)...);
        assert(system != nullptr);
        m_systems.push_back(system);
        return id;
    }

    template <typename TSystem>
    SystemTypeId GetTypeId () const {
        auto it = m_typeToId.find(typeid(std::decay_t<TSystem>));
        assert(it != m_typeToId.cend());
        return it->second;
    }

    bool HasType (SystemTypeId id) const { return id < Count(); }

    template <typename TSystem>
    bool HasType () { return HasType(GetTypeId<TSystem>()); }

    uint32_t Count () const { return static_cast<uint32_t>(m_typeToId.size()); }
    System* GetSystem (SystemTypeId index) { return m_systems[index]; }
    const System* GetSystem (SystemTypeId index) const { return m_systems[index]; }

    SystemType& GetSystemType (SystemTypeId index) { return m_systemTypes[index]; }
    const SystemType& GetSystemType (SystemTypeId index) const { return m_systemTypes[index]; }

private:
    std::vector<System*> m_systems{};
    std::vector<SystemType> m_systemTypes;
    std::map<std::type_index, SystemTypeId> m_typeToId{};
};

} // namespace ecs