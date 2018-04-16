#include "Mesh.h"
#include <glad/glad.h>

CMesh::CMesh (uint32_t vao, uint32_t vbo, uint32_t ebo, uint32_t indexCount) :
    m_vao{vao},
    m_vbo{vbo},
    m_ebo{ebo},
    m_indexCount{indexCount} {
}

void CMesh::Draw () const {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
