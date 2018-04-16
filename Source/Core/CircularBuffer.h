#pragma once

#include "Core/CoreDebug.h"
#include <array>

template<typename T, size_t BufferSize>
class CCircularBuffer {
public:
    size_t Size () const { return (m_end > m_begin) ? (m_end - m_begin) : (Size - m_begin + m_end); }
    bool Empty () const { return m_begin == m_end; }

    void Push (T&& obj) {
        if ((m_end + 1) % BufferSize != m_begin) {
            m_queue[m_end] = std::forward<T>(obj);
            m_end = (m_end + 1) % BufferSize;
        }
    }

    void Pop () {
        Assert_(!Empty());
        m_begin = (m_begin + 1) % BufferSize;
    }

    T& Peek () { Assert_(!Empty()); return m_queue[m_begin]; }
    const T& Peek () const { Assert_(!Empty()); return m_queue[m_begin]; }

private:
    std::array<T, BufferSize> m_queue;
    size_t m_begin{};
    size_t m_end{};
};
