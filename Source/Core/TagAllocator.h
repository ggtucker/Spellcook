#pragma once

#include "CoreDebug.h"
#include <utility>
#include <vector>

template <typename TObject> class CTagAllocator;

template <typename TObject>
class CTag {
public:
    CTag () = default;
    CTag (const CTag& other) : m_allocator{other.m_allocator}, m_index{other.m_index}, m_counter{other.m_counter} { IncRefCount(); }
    CTag& operator= (const CTag& other) {
        if (*this != other) {
            DecRefCount();
            m_index = other.m_index;
            m_counter = other.m_counter;
            m_allocator = other.m_allocator;
            IncRefCount();
        }
        return *this;
    }
    virtual ~CTag () { DecRefCount(); }

    bool IsAssigned () const { return m_counter > 0; }
    bool IsValid () const { return Deref() != nullptr; }

    TObject* Deref () const { return m_allocator ? m_allocator->Deref(*this) : nullptr; }

    bool operator== (const CTag& other) const { return m_index == other.m_index && m_counter == other.m_counter; }
    bool operator!= (const CTag& other) const { return m_index != other.m_index || m_counter != other.m_counter; }

private:
    friend class CTagAllocator<TObject>;

    CTag (CTagAllocator<TObject>* allocator, uint32_t index, uint32_t counter) : m_allocator{allocator}, m_index {index}, m_counter{counter} {}
    void IncRefCount () { if (m_allocator) m_allocator->IncrementRefCount(*this); }
    void DecRefCount () { if (m_allocator) m_allocator->DecrementRefCount(*this); }

    CTagAllocator<TObject>* m_allocator{nullptr};
    uint32_t m_index{0u};
    uint32_t m_counter{0u};
};

template <typename TObject>
class CTagAllocator {
public:
    explicit CTagAllocator (uint32_t capacity) { InitPool(capacity); }

    CTagAllocator (const CTagAllocator&) = delete;
    CTagAllocator (CTagAllocator&&) = delete;
    CTagAllocator& operator= (const CTagAllocator&) = delete;
    CTagAllocator& operator= (CTagAllocator&&) = delete;

    TObject* Deref (const CTag<TObject>& tag) {
        PoolNode& node = m_pool[tag.m_index];
        if (node.m_counter == tag.m_counter) {
            return &node.m_union.m_content.m_object;
        }
        return nullptr;
    }

    template <typename... TArgs>
    CTag<TObject> Create (TArgs&&... args) {
        Assert_(m_firstAvailable);
        const CTag<TObject> tag(this, m_firstAvailable - m_pool.data(), ++m_firstAvailable->m_counter);

        RefCountObject* obj = reinterpret_cast<RefCountObject*>(&m_firstAvailable->m_union);
        m_firstAvailable = m_firstAvailable->m_union.m_next;

        new(&obj->m_object) TObject(std::forward<TArgs>(args)...);
        ++obj->m_refCount;

        return tag;
    }

    void Destroy (const CTag<TObject>& tag) {
        PoolNode& node = m_pool[tag.m_index];
        if (node.m_counter == tag.m_counter) {
            ++node.m_counter;
            node.m_union.m_content.m_refCount = 0;
            node.m_union.m_content.m_object.~TObject();
            node.m_union.m_next = m_firstAvailable;
            m_firstAvailable = &node;
        }
    }

private:
    friend class CTag<TObject>;

    void IncrementRefCount (const CTag<TObject>& tag) {
        PoolNode& node = m_pool[tag.m_index];
        if (node.m_counter == tag.m_counter) {
            ++node.m_union.m_content.m_refCount;
        }
    }

    void DecrementRefCount (const CTag<TObject>& tag) {
        PoolNode& node = m_pool[tag.m_index];
        if (node.m_counter == tag.m_counter) {
            --node.m_union.m_content.m_refCount;
            if (node.m_union.m_content.m_refCount == 0) {
                Destroy(tag);
            }
        }
    }

    struct RefCountObject {
        TObject m_object;
        uint32_t m_refCount;
    };
    struct PoolNode {
        union NodeUnion {
            RefCountObject m_content;
            PoolNode* m_next;
            NodeUnion () {}
            ~NodeUnion () {}
        } m_union;
        uint32_t m_counter;
        PoolNode () {}
        ~PoolNode () {}
    };
    std::vector<PoolNode> m_pool;
    PoolNode* m_firstAvailable;

    void InitPool (uint32_t capacity) {
        m_pool.resize(capacity);
        m_firstAvailable = &m_pool[0];
        for (size_t i = 0; i < m_pool.size() - 1; ++i) {
            m_pool[i].m_union.m_next = &m_pool[i + 1];
        }
        m_pool[m_pool.size() - 1].m_union.m_next = nullptr;
    }
};