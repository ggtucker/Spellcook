#pragma once

#include "BlockObjectPool.h"
#include "ComponentType.h"
#include "EncosysTypes.h"
#include <array>
#include <cassert>
#include <map>
#include <typeindex>

namespace ecs {

class ComponentRegistry {
public:
    ComponentRegistry () {
        ComponentList::ForTypes([this](auto t) {
            using TComponent = std::decay_t<TYPE_OF(t)>;
            const ComponentTypeId id = Count();
            m_componentTypes[id] = ComponentType(id, sizeof(TComponent));
            m_typeToId[typeid(TComponent)] = id;

            m_componentPools[id] = new BlockObjectPool<TComponent>();
            assert(m_componentPools[id] != nullptr);
        });
    }

    virtual ~ComponentRegistry () {
        for (uint32_t i = 0; i < Count(); ++i) {
            delete m_componentPools[i];
        }
    }

    template <typename TComponent>
    static constexpr ComponentTypeId GetTypeId () {
        static_assert(ComponentList::Contains<std::decay_t<TComponent>>(), "Component type was not registered.");
        return ComponentList::IndexOf<std::decay_t<TComponent>>();
    }

    template <typename TComponent>
    const ComponentType& GetType () const {
        return m_componentTypes[GetTypeId<TComponent>()];
    }

    const ComponentType& GetType (ComponentTypeId id) const {
        assert(id < Count());
        return m_componentTypes[id];
    }

    template <typename TComponent>
    auto& GetStorage () { return static_cast<BlockObjectPool<std::decay_t<TComponent>>&>(GetStorage(GetTypeId<TComponent>())); }

    template <typename TComponent>
    const auto& GetStorage () const { return static_cast<const BlockObjectPool<std::decay_t<TComponent>>&>(GetStorage(GetTypeId<TComponent>())); }

    BlockMemoryPool& GetStorage (ComponentTypeId id) { assert(id < Count()); return *m_componentPools[id]; }
    const BlockMemoryPool& GetStorage (ComponentTypeId id) const { assert(id < Count()); return *m_componentPools[id]; }

    uint32_t Count () const { return static_cast<uint32_t>(m_typeToId.size()); }
    const ComponentType& operator[] (uint32_t index) const { return m_componentTypes[index]; }

private:
    std::array<BlockMemoryPool*, ComponentList::Size> m_componentPools{};
    std::array<ComponentType, ComponentList::Size> m_componentTypes;
    std::map<std::type_index, ComponentTypeId> m_typeToId{};
};

} // namespace ecs