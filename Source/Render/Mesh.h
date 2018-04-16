#pragma once

#include "Core/CoreTypes.h"

class CMesh {
public:
    CMesh () = default;
    CMesh (uint32_t vao, uint32_t vbo, uint32_t ebo, uint32_t indexCount);

    void Draw () const;

private:
    uint32_t m_vao{};
    uint32_t m_vbo{};
    uint32_t m_ebo{};
    uint32_t m_indexCount{};
};
