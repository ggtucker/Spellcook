#include "Chunk.h"

CChunk::CChunk () :
    m_cubes{new CCube[c_chunkLength * c_chunkLength * c_chunkHeight]} {
}

CChunk::~CChunk () {
    delete[] m_cubes;
}

CCube& CChunk::Cube (uint32_t x, uint32_t y, uint32_t z) {
    return m_cubes[x + c_chunkLength * (y + c_chunkHeight * z)];
}