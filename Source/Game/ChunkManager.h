#pragma once

#include "Chunk.h"
#include <map>
#include <mutex>
#include <thread>

class CChunkManager {
public:
    CChunkManager ();
    ~CChunkManager ();

private:
    static void StaticThreadUpdate (void* data);
    void ThreadUpdate ();

    float m_loadRadius{120.f};

    // Chunk map
    std::map<SChunkKey, CChunk*> m_chunkMap{};
    std::mutex m_chunkMapMutex{};

    // Update thread
    std::thread* m_updateThread{nullptr};
    bool m_updateThreadActive{true};
    bool m_updateThreadFinished{false};
};
