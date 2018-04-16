#pragma once

#include "Core/ConcurrentQueue.h"
#include "Chunk.h"
#include <map>
#include <mutex>
#include <thread>

static constexpr uint32_t s_chunkLoadQueueSize = 64;
using CChunkLoadQueue = CConcurrentQueue<CChunk*, s_chunkLoadQueueSize>;

class CChunkLoader {
public:
    void Initialize (CChunkLoadQueue& loadQueue);
    void Terminate ();

private:
    static void StaticThreadUpdate (void* data);
    void ThreadUpdate ();

    CChunkLoadQueue* m_loadQueue{nullptr};

    std::thread* m_thread{nullptr};
    bool m_threadActive{true};
    bool m_threadFinished{false};
};
