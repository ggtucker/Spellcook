#include "MeshBuilder.h"
#include <glad/glad.h>

void CMeshBuilder::Clear () {
    m_vertices.clear();
    m_indices.clear();
}

CMesh CMeshBuilder::Build () const {
    if (m_vertices.empty() || m_indices.empty()) {
        return CMesh();
    }
    uint32_t vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return CMesh(vao, vbo, ebo, static_cast<uint32_t>(m_indices.size()));
}

uint32_t CMeshBuilder::AddVertex (const SVertex& vertex) {
    m_vertices.push_back(vertex);
    return static_cast<uint32_t>(m_vertices.size() - 1);
}

void CMeshBuilder::AddTriangle (uint32_t vertex1, uint32_t vertex2, uint32_t vertex3) {
    m_indices.push_back(vertex1);
    m_indices.push_back(vertex2);
    m_indices.push_back(vertex3);
}