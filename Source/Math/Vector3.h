#pragma once

#include "Core/CoreDebug.h"
#include "Core/CoreTypes.h"
#include <ostream>

namespace math {

template <typename T>
class CVector3T {
public:
    union {
        struct { T x, y, z; };
        T xyz[3];
    };

    CVector3T () : x{}, y{}, z{} {}
    CVector3T (const T& x, const T& y) : x{x}, y{y}, z{} {}
    CVector3T (const T& x, const T& y, const T& z) : x{x}, y{y}, z{z} {}

    CVector3T Cross (const CVector3T& b) const { return CVector3T<T>(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x); }
    T Dot (const CVector3T& b) const { return x * b.x + y * b.y + z * b.z; }
    T SquaredMag () const { return x * x + y * y + z * z; }
    T Mag () const { return Sqrt(SquaredMag()); }
    bool IsZero () const { return x == 0 && y == 0 && z == 0; }
    void Normalize () {
        const T magnitude = Mag();
        if (magnitude > 0) {
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
        }
    }

    // Bracket operators
    T& operator[] (uint32_t index) { Assert_(index < 3); return xyz[index]; }
    const T& operator[] (uint32_t index) const { Assert_(index < 3); return xyz[index]; }

    // Arithmetic operators
    CVector3T operator+ () const { return *this; }
    CVector3T operator- () const { return CVector3T(-x, -y, -z); }
    friend CVector3T operator+ (const CVector3T& a, const CVector3T& b) { return CVector3T(a.x + b.x, a.y + b.y, a.z + b.z); }
    friend CVector3T operator- (const CVector3T& a, const CVector3T& b) { return CVector3T(a.x - b.x, a.y - b.y, a.z - b.z); }
    friend CVector3T operator* (const CVector3T& a, const T& s) { return CVector3T(a.x * s, a.y * s, a.z * s); }
    friend CVector3T operator/ (const CVector3T& a, const T& s) { Assert_(s != 0); return CVector3T(a.x / s, a.y / s, a.z / s); }

    // Assignment operators
    CVector3T& operator+= (const CVector3T& b) { *this = *this + b; return *this; }
    CVector3T& operator-= (const CVector3T& b) { *this = *this - b; return *this; }
    CVector3T& operator*= (const T& s) { *this = *this * s; return *this; }
    CVector3T& operator/= (const T& s) { *this = *this / s; return *this; }

    // Comparison operators
    friend bool operator== (const CVector3T& a, const CVector3T& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
    friend bool operator!= (const CVector3T& a, const CVector3T& b) { return a.x != b.x || a.y != b.y || a.z != b.z; }

    // Stream operators
    friend std::ostream& operator<< (std::ostream& stream, const CVector3T& a) { stream << "(" << a.x << ", " << a.y << ", " << a.z << ")"; return stream; }
};

template <typename T>
CVector3T<T> Normalize (const CVector3T<T>& a) { CVector3T<T> b = a; b.Normalize(); return b; }

template <typename T>
CVector3T<T> Cross (const CVector3T<T>& a, const CVector3T<T>& b) { return a.Cross(b); }

using Vec3 = CVector3T<float>;
using Vec3f = CVector3T<CFixed>;

} // namespace math