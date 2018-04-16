#pragma once

#include "Core/CoreTypes.h"
#include "Math/Vector3.h"
#include "Cube.h"

struct SChunkKey : public math::Vec3i {
    bool operator< (const SChunkKey& other) const {
        if (x < other.x) return true;
        if (x > other.x) return false;
        if (y < other.y) return true;
        if (y > other.y) return false;
        if (z < other.z) return true;
        if (z > other.z) return false;
        return false;
    }
};

enum class EChunkState {
    Uninitialized,
    Loading,
    Unloading,
    RebuildingMesh,
    RebuildComplete,
    Idle
};

class CChunk {
public:
    static constexpr uint32_t c_chunkLength = 32;
    static constexpr uint32_t c_chunkHeight = 64;

    CChunk ();
    ~CChunk ();

private:
    CCube& Cube (uint32_t x, uint32_t y, uint32_t z);

    CCube* m_cubes;
};
