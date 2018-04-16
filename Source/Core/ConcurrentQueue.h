#pragma once

#include <cstdint>
#include <array>
#include <mutex>
#include "CircularBuffer.h"

template <typename T, size_t Size>
class CConcurrentQueue {
public:

    void Push (const T& task) {
        std::unique_lock<std::mutex> lock(m_mutex, std::defer_lock);
        m_notifier.wait(lock, [this]() { return m_tasks.size() < Size; });
        m_tasks.Push(task);
        lock.unlock();
        m_notifier.notify_all();
    }

    T Pop () {
        std::unique_lock<std::mutex> lock(m_mutex, std::defer_lock);
        m_notifier.wait(lock, [this]() { return !m_tasks.Empty(); });
        T task = m_tasks.Peek();
        m_tasks.Pop();
        lock.unlock();
        m_notifier.notify_all();
        return task;
    }

private:
    CCircularBuffer<T, Size> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_notifier;
};
