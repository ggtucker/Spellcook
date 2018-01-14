#pragma once

#include "Time.h"

class CTimer {
public:
    CTimer ();

    CTime TimeElapsed () const;
    CTime Restart ();

private:
    CTime m_startTime{};
};