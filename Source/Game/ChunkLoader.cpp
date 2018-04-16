#include "ChunkLoader.h"

void CChunkLoader::Initialize (CChunkLoadQueue& loadQueue) {
    m_loadQueue = &loadQueue;
    m_thread = new std::thread(StaticThreadUpdate, this);
}

void CChunkLoader::Terminate () {
    m_threadActive = false;
    while (!m_threadFinished) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    m_loadQueue = nullptr;
}

void CChunkLoader::StaticThreadUpdate (void* data) {
    static_cast<CChunkLoader*>(data)->ThreadUpdate();
}

void CChunkLoader::ThreadUpdate () {
    while (m_threadActive) {
        CChunk* chunk = m_loadQueue->Pop();
    }
    m_threadFinished = true;
}
