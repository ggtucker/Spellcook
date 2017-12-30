#pragma once

#include "Core/CoreDebug.h"
#include "Core/CoreTypes.h"
#include <ostream>

namespace math {

template <typename T>
class CVector4T {
public:
    union {
        struct { T x, y, z, w; };
        T xyzw[4];
    };

    CVector4T () : x{}, y{}, z{}, w{} {}
    CVector4T (const T& x, const T& y) : x{x}, y{y}, z{}, w{} {}
    CVector4T (const T& x, const T& y, const T& z) : x{x}, y{y}, z{z}, w{} {}
    CVector4T (const T& x, const T& y, const T& z, const T& w) : x{x}, y{y}, z{z}, w{w} {}

    T Dot (const CVector4T& b) const { return x * b.x + y * b.y + z * b.z + w * b.w; }
    T SquaredMag () const { return x * x + y * y + z * z + w * w; }
    T Mag () const { return Sqrt(SquaredMag()); }
    bool IsZero () const { return x == 0 && y == 0 && z == 0 && w == 0; }
    void Normalize () {
        const T magnitude = Mag();
        if (magnitude > 0) {
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
            w /= magnitude;
        }
    }

    // Bracket operators
    T& operator[] (uint32_t index) { Assert_(index < 4); return xyzw[index]; }
    const T& operator[] (uint32_t index) const { Assert_(index < 4); return xyzw[index]; }

    // Arithmetic operators
    CVector4T operator+ () const { return *this; }
    CVector4T operator- () const { return CVector4T(-x, -y, -z, -w); }
    friend CVector4T operator+ (const CVector4T& a, const CVector4T& b) { return CVector4T(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
    friend CVector4T operator- (const CVector4T& a, const CVector4T& b) { return CVector4T(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
    friend CVector4T operator* (const CVector4T& a, const T& s) { return CVector4T(a.x * s, a.y * s, a.z * s, a.w * s); }
    friend CVector4T operator/ (const CVector4T& a, const T& s) { Assert_(s != 0); return CVector4T(a.x / s, a.y / s, a.z / s, a.w / s); }

    // Assignment operators
    CVector4T& operator+= (const CVector4T& b) { *this = *this + b; return *this; }
    CVector4T& operator-= (const CVector4T& b) { *this = *this - b; return *this; }
    CVector4T& operator*= (const T& s) { *this = *this * s; return *this; }
    CVector4T& operator/= (const T& s) { *this = *this / s; return *this; }

    // Comparison operators
    friend bool operator== (const CVector4T& a, const CVector4T& b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
    friend bool operator!= (const CVector4T& a, const CVector4T& b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }

    // Stream operators
    friend std::ostream& operator<< (std::ostream& stream, const CVector4T& a) { stream << "(" << a.x << ", " << a.y << ", " << a.z << ", " << a.w << ")"; return stream; }
};

template <typename T>
CVector4T<T> Normalize (const CVector4T<T>& a) { CVector4T<T> b = a; b.Normalize(); return b; }

using CVector4 = CVector4T<float>;
using CVector4f = CVector4T<CFixed>;

} // namespace math