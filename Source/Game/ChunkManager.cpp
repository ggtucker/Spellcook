#include "ChunkManager.h"

CChunkManager::CChunkManager () :
    m_updateThread{new std::thread(StaticThreadUpdate, this)} {
}

CChunkManager::~CChunkManager () {
    m_updateThreadActive = false;
    while (!m_updateThreadFinished) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void CChunkManager::StaticThreadUpdate (void* data) {
    static_cast<CChunkManager*>(data)->ThreadUpdate();
}

void CChunkManager::ThreadUpdate () {
    while (m_updateThreadActive) {

    }
}