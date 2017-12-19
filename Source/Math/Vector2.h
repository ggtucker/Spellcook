#pragma once

#include "Core/CoreDebug.h"
#include "Core/CoreTypes.h"
#include <ostream>

template <typename T>
class CVector2 {
public:
    T x, y;

    CVector2 () : x{}, y{} {}
    CVector2 (const T& x, const T& y) : x{x}, y{y} {}

    T Dot (const CVector2& b) const { return x * b.x + y * b.y; }
    T SquaredMag () const { return x * x + y * y; }
    T Mag () const { return Sqrt(SquaredMag()); }
    bool IsZero () const { return x == 0 && y == 0; }
    void Normalize () {
        const T magnitude = Mag();
        if (magnitude > 0) {
            x /= magnitude;
            y /= magnitude;
        }
    }

    // Arithmetic operators
    CVector2 operator+ () const { return *this; }
    CVector2 operator- () const { return CVector2(-x, -y); }
    friend CVector2 operator+ (const CVector2& a, const CVector2& b) { return CVector2(a.x + b.x, a.y + b.y); }
    friend CVector2 operator- (const CVector2& a, const CVector2& b) { return CVector2(a.x - b.x, a.y - b.y); }
    friend CVector2 operator* (const CVector2& a, const T& s) { return CVector2(a.x * s, a.y * s); }
    friend CVector2 operator/ (const CVector2& a, const T& s) { Assert_(s != 0); return CVector2(a.x / s, a.y / s); }

    // Assignment operators
    CVector2& operator+= (const CVector2& b) { *this = *this + b; return *this; }
    CVector2& operator-= (const CVector2& b) { *this = *this - b; return *this; }
    CVector2& operator*= (const T& s) { *this = *this * s; return *this; }
    CVector2& operator/= (const T& s) { *this = *this / s; return *this; }

    // Comparison operators
    friend bool operator== (const CVector2& a, const CVector2& b) { return a.x == b.x && a.y == b.y; }
    friend bool operator!= (const CVector2& a, const CVector2& b) { return a.x != b.x || a.y != b.y; }

    // Stream operators
    friend std::ostream& operator<< (std::ostream& stream, const CVector2& a) { stream << "(" << a.x << ", " << a.y << ")"; return stream; }
};

template <typename T>
CVector2<T> Normalize (const CVector2<T>& a) { CVector2<T> b = a; b.Normalize(); return b; }

using CVector2f = CVector2<CFixed>;
