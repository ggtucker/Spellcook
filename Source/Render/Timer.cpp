#include "Timer.h"

#include <GLFW/glfw3.h>

static CTime GetCurrentTime () { return CTime::Seconds(static_cast<float>(glfwGetTime())); }

CTimer::CTimer () : m_startTime{GetCurrentTime()} {
}

CTime CTimer::TimeElapsed () const {
    return GetCurrentTime() - m_startTime;
}

CTime CTimer::Restart () {
    const CTime currentTime = GetCurrentTime();
    const CTime elapsed = currentTime - m_startTime;
    m_startTime = currentTime;
    return elapsed;
}
