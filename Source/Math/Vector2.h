#pragma once

#include "Core/CoreDebug.h"
#include "Core/CoreTypes.h"
#include "MathTypes.h"
#include <ostream>

namespace math {

template <typename T>
class CVector2T {
public:
    union {
        struct { T x, y; };
        T xy[2];
    };

    CVector2T () : x{}, y{} {}
    CVector2T (const T& x, const T& y) : x{x}, y{y} {}

    template <typename U>
    CVector2T<U> Cast () const {
        return CVector2T<U>{
            static_cast<U>(x),
            static_cast<U>(y)
        };
    }

    T Dot (const CVector2T& b) const { return x * b.x + y * b.y; }
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

    // Bracket operators
    T& operator[] (uint32_t index) { Assert_(index < 2); return xy[index]; }
    const T& operator[] (uint32_t index) const { Assert_(index < 2); return xy[index]; }

    // Arithmetic operators
    CVector2T operator+ () const { return *this; }
    CVector2T operator- () const { return CVector2T(-x, -y); }
    friend CVector2T operator+ (const CVector2T& a, const CVector2T& b) { return CVector2T(a.x + b.x, a.y + b.y); }
    friend CVector2T operator- (const CVector2T& a, const CVector2T& b) { return CVector2T(a.x - b.x, a.y - b.y); }
    friend CVector2T operator* (const CVector2T& a, const T& s) { return CVector2T(a.x * s, a.y * s); }
    friend CVector2T operator* (const T& s, const CVector2T& a) { return CVector2T(a.x * s, a.y * s); }
    friend CVector2T operator/ (const CVector2T& a, const T& s) { Assert_(s != 0); return CVector2T(a.x / s, a.y / s); }

    // Assignment operators
    CVector2T& operator+= (const CVector2T& b) { *this = *this + b; return *this; }
    CVector2T& operator-= (const CVector2T& b) { *this = *this - b; return *this; }
    CVector2T& operator*= (const T& s) { *this = *this * s; return *this; }
    CVector2T& operator/= (const T& s) { *this = *this / s; return *this; }

    // Comparison operators
    friend bool operator== (const CVector2T& a, const CVector2T& b) { return a.x == b.x && a.y == b.y; }
    friend bool operator!= (const CVector2T& a, const CVector2T& b) { return a.x != b.x || a.y != b.y; }

    // Stream operators
    friend std::ostream& operator<< (std::ostream& stream, const CVector2T& a) { stream << "(" << a.x << ", " << a.y << ")"; return stream; }
};

template <typename T>
T Dot (const CVector2T<T>& a, const CVector2T<T>& b) { return a.Dot(b); }

template <typename T>
CVector2T<T> Normalize (const CVector2T<T>& a) { CVector2T<T> b = a; b.Normalize(); return b; }

using Vec2 = CVector2T<float>;
using Vec2f = CVector2T<CFixed>;
using Vec2i = CVector2T<int32_t>;

} // namespace math