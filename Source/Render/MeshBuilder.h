#pragma once

#include "Vertex.h"
#include "Mesh.h"
#include <vector>

class CMeshBuilder {
public:
    void Clear ();
    CMesh Build () const;

    uint32_t AddVertex (const SVertex& vertex);
    void AddTriangle (uint32_t vertex1, uint32_t vertex2, uint32_t vertex3);

private:
    std::vector<SVertex> m_vertices;
    std::vector<uint32_t> m_indices;
};
