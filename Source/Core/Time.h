#pragma once

#include "Core/CoreDebug.h"
#include "Core/CoreTypes.h"

class CTime {
public:
    CTime () = default;

    float Seconds () const { return m_microseconds / 1000000.f; }
    int32_t Milliseconds () const { return static_cast<int32_t>(m_microseconds / 1000); }
    int64_t Microseconds () const { return m_microseconds; }

    static CTime Seconds (float seconds) { CTime time; time.m_microseconds = static_cast<int64_t>(seconds * 1000000); return time; }
    static CTime Milliseconds (int32_t milliseconds) { CTime time; time.m_microseconds = milliseconds * 1000; return time; }
    static CTime Microseconds (int64_t microseconds) { CTime time; time.m_microseconds = microseconds; return time; }

    // Arithmetic operators
    CTime operator+ () const { return *this; }
    CTime operator- () const { return CTime::Microseconds(-m_microseconds); }
    friend CTime operator+ (const CTime& a, const CTime& b) { return CTime::Microseconds(a.m_microseconds + b.m_microseconds); }
    friend CTime operator- (const CTime& a, const CTime& b) { return CTime::Microseconds(a.m_microseconds - b.m_microseconds); }
    friend CTime operator* (const CTime& a, const float s) { return CTime::Seconds(a.Seconds() * s); }
    friend CTime operator* (const float s, const CTime& a) { return a * s; }
    friend CTime operator* (const CTime& a, const int64_t s) { return CTime::Microseconds(a.m_microseconds * s); }
    friend CTime operator* (const int64_t s, const CTime& a) { return a * s; }
    friend CTime operator/ (const CTime& a, const float s) { Assert_(s != 0); return CTime::Seconds(a.Seconds() / s); }
    friend CTime operator/ (const CTime& a, const int64_t s) { Assert_(s != 0); return CTime::Microseconds(a.m_microseconds / s); }

    // Assignment operators
    CTime& operator+= (const CTime& b) { *this = *this + b; return *this; }
    CTime& operator-= (const CTime& b) { *this = *this - b; return *this; }
    CTime& operator*= (const float s) { *this = *this * s; return *this; }
    CTime& operator*= (const int64_t s) { *this = *this * s; return *this; }
    CTime& operator/= (const float s) { *this = *this / s; return *this; }
    CTime& operator/= (const int64_t s) { *this = *this / s; return *this; }

    // Comparison operators
    friend bool operator== (const CTime& a, const CTime& b) { return a.m_microseconds == b.m_microseconds; }
    friend bool operator!= (const CTime& a, const CTime& b) { return a.m_microseconds != b.m_microseconds; }
    friend bool operator< (const CTime& a, const CTime& b) { return a.m_microseconds < b.m_microseconds; }
    friend bool operator<= (const CTime& a, const CTime& b) { return a.m_microseconds <= b.m_microseconds; }
    friend bool operator> (const CTime& a, const CTime& b) { return a.m_microseconds > b.m_microseconds; }
    friend bool operator>= (const CTime& a, const CTime& b) { return a.m_microseconds >= b.m_microseconds; }

private:
    int64_t m_microseconds{};
};