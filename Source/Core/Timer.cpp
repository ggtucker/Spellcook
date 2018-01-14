#include "Timer.h"

#include <chrono>

static const std::chrono::system_clock::time_point s_programStartTime = std::chrono::system_clock::now();

static CTime GetCurrentTime () {
    return CTime::Seconds(std::chrono::duration<float>(std::chrono::system_clock::now() - s_programStartTime).count());
}

CTimer::CTimer () : m_startTime{GetCurrentTime()} {}

CTime CTimer::TimeElapsed () const {
    return GetCurrentTime() - m_startTime;
}

CTime CTimer::Restart () {
    const CTime currentTime = GetCurrentTime();
    const CTime elapsed = currentTime - m_startTime;
    m_startTime = currentTime;
    return elapsed;
}
