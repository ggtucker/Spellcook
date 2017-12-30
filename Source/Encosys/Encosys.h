#pragma once

#include "ComponentRegistry.h"
#include "EncosysTypes.h"
#include "EntityId.h"
#include "FunctionTraits.h"
#include "SystemRegistry.h"
#include <array>
#include <cassert>
#include <unordered_map>
#include <vector>

namespace ecs {

class Encosys;

class EntityStorage {
public:
    explicit EntityStorage        (EntityId id) : m_id{id} {}

    EntityId GetId                () const { return m_id; }

    bool     HasComponent         (ComponentTypeId typeId) const { return m_bitset[typeId]; }
    bool     HasComponentBitset   (const ComponentBitset& bitset) const { return (m_bitset & bitset) == bitset; }

    uint32_t GetComponentIndex    (ComponentTypeId typeId) const { return m_components[typeId]; }
    void     SetComponentIndex    (ComponentTypeId typeId, uint32_t index) { m_bitset.set(typeId); m_components[typeId] = index; }
    void     RemoveComponentIndex (ComponentTypeId typeId) { m_bitset.set(typeId, false); m_components[typeId] = c_invalidIndex; }

private:
    EntityId m_id;
    ComponentBitset m_bitset;
    std::array<uint32_t, ComponentList::Size> m_components;
};

class Entity {
public:
    Entity (Encosys& encosys, EntityStorage& storage) : m_encosys{encosys}, m_storage{storage} {}

    EntityId                                                      GetId              () const { return m_storage.GetId(); }

    bool                                                          HasComponentBitset (const ComponentBitset& bitset) const { return m_storage.HasComponentBitset(bitset); }
    template <typename TComponent> bool                           HasComponent       () const { return m_storage.HasComponent(m_encosys.GetComponentType<TComponent>().Id()); }

    // Template component members
    template <typename TComponent, typename... TArgs> TComponent& AddComponent       (TArgs&&... args);
    template <typename TComponent> void                           RemoveComponent    ();
    template <typename TComponent> TComponent*                    GetComponent       ();
    template <typename TComponent> const TComponent*              GetComponent       () const;

private:
    Encosys& m_encosys;
    EntityStorage& m_storage;
};

class Encosys {
public:
    // Constructors
    Encosys () = default;

    // Entity members
    EntityId                                                      Create               (bool active = true);
    EntityId                                                      Copy                 (EntityId e, bool active = true);
    void                                                          Destroy              (EntityId e);
    Entity                                                        Get                  (EntityId e);
    bool                                                          IsValid              (EntityId e) const;
    bool                                                          IsActive             (EntityId e) const;
    void                                                          SetActive            (EntityId e, bool active);
    uint32_t                                                      EntityCount          () const;
    uint32_t                                                      ActiveEntityCount    () const;

    // Component members
    template <typename TComponent> const ComponentType&           GetComponentType     () const;
    const ComponentType&                                          GetComponentType     (ComponentTypeId typeId) const;

    // System members
    template <typename TSystem, typename... TArgs> void           RegisterSystem       (TArgs&&... args);
    void                                                          Update               (TimeDelta delta);

    // Iteration members
    template <typename TCallback> void                            ForEach              (TCallback&& callback);
    Entity                                                        operator[]           (uint32_t index) { return Entity(*this, m_entities[index]); }

private:
    friend class Entity;

    template <typename TComponentDependencyList>
    friend class SystemContext;

    // Helper members
    void IndexSwapEntities (uint32_t lhsIndex, uint32_t rhsIndex);
    bool IndexIsActive (uint32_t index) const;
    void IndexSetActive (uint32_t& index, bool active);

    template <typename TCallback, typename... Args, std::size_t... Seq>
    void UnpackAndCallback (EntityStorage& entity, TCallback&& callback, tmp::TypeList<Args...>, tmp::Sequence<Seq...>);

    // Member variables
    ComponentRegistry m_componentRegistry;
    SystemRegistry m_systemRegistry;
    std::unordered_map<EntityId, uint32_t> m_idToEntity;
    std::vector<EntityStorage> m_entities;
    uint32_t m_entityIdCounter{};
    uint32_t m_entityActiveCount{};
};

template <typename TComponent>
TComponent* Entity::GetComponent () {
    return const_cast<TComponent*>(static_cast<const Entity*>(this)->GetComponent<TComponent>());
}

template <typename TComponent>
const TComponent* Entity::GetComponent () const {
    const ComponentTypeId typeId = m_encosys.GetComponentType<TComponent>().Id();
    if (!m_storage.HasComponent(typeId)) {
        return nullptr;
    }
    const auto& container = m_encosys.m_componentRegistry.GetStorage<TComponent>();
    return &container.GetObject(m_storage.GetComponentIndex(typeId));
}

template <typename TComponent, typename... TArgs>
TComponent& Entity::AddComponent (TArgs&&... args) {
    const ComponentTypeId typeId = m_encosys.m_componentRegistry.GetTypeId<TComponent>();
    auto& container = m_encosys.m_componentRegistry.GetStorage<TComponent>();
    uint32_t componentIndex = container.Construct(std::forward<TArgs>(args)...);
    m_storage.SetComponentIndex(typeId, componentIndex);
    return container.GetObject(componentIndex);
}

template <typename TComponent>
void Entity::RemoveComponent () {
    const ComponentTypeId typeId = m_componentRegistry.GetTypeId<TComponent>();
    if (m_storage.HasComponent(typeId)) {
        auto& container = m_encosys.m_componentRegistry.GetStorage<TComponent>();
        container.Destroy(m_storage.GetComponentIndex(typeId));
        m_storage.RemoveComponentIndex(typeId);
    }
}

template <typename TComponent>
const ComponentType& Encosys::GetComponentType () const {
    return m_componentRegistry.GetType<TComponent>();
}

template <typename TSystem, typename... TArgs>
void Encosys::RegisterSystem (TArgs&&... args) {
    ecs::SystemTypeId systemId = m_systemRegistry.Register<TSystem>(std::forward<TArgs>(args)...);
    m_systemRegistry.GetSystemType(systemId).SetRequiredComponents(
        m_systemRegistry.GetSystem(systemId)->GetRequiredComponents(m_componentRegistry)
    );
}

template <typename TCallback>
void Encosys::ForEach (TCallback&& callback) {
    using FTraits = tmp::FunctionTraits<decltype(callback)>;
    static_assert(FTraits::ArgCount > 0, "First callback param must be ecs::Entity.");
    static_assert(std::is_same<std::decay_t<typename FTraits::Arg<0>>, Entity>::value, "First callback param must be ecs::Entity.");
    using FComponentArgs = typename FTraits::Args::RemoveFirst;

    ComponentBitset targetMask{};
    FComponentArgs::ForTypes([this, &targetMask] (auto t) {
        (void)t;
        using TComponent = TYPE_OF(t);
        static_assert(ComponentList::Contains<std::decay_t<TComponent>>(), "Component type was not registered.");
        targetMask.set(ComponentRegistry::GetTypeId<TYPE_OF(t)>());
    });

    for (uint32_t i = 0; i < m_entityActiveCount; ++i) {
        EntityStorage& entity = m_entities[i];
        if (entity.HasComponentBitset(targetMask)) {
            UnpackAndCallback(
                entity,
                callback,
                FComponentArgs{},
                typename tmp::GenerateSequence<FTraits::ArgCount>::Type{}
            );
        }
    }
}

template <typename TCallback, typename... Args, std::size_t... Seq>
void Encosys::UnpackAndCallback (EntityStorage& entity, TCallback&& callback, tmp::TypeList<Args...>, tmp::Sequence<Seq...>) {
    auto params = std::make_tuple(
        Entity(*this, entity),
        std::ref(m_componentRegistry.GetStorage<std::decay_t<Args>>().GetObject(entity.GetComponentIndex(m_componentRegistry.GetTypeId<std::decay_t<Args>>())))...
    );
    callback(std::get<Seq>(params)...);
}

} // namespace ecs
