#pragma once

#include "Core/CoreDebug.h"
#include "Core/CoreTypes.h"
#include <ostream>

template <typename T>
class CVector4 {
public:
    T x, y, z, w;

    CVector4 () : x{}, y{}, z{}, w{} {}
    CVector4 (const T& x, const T& y) : x{x}, y{y}, z{}, w{} {}
    CVector4 (const T& x, const T& y, const T& z) : x{x}, y{y}, z{z}, w{} {}
    CVector4 (const T& x, const T& y, const T& z, const T& w) : x{x}, y{y}, z{z}, w{w} {}

    T Dot (const CVector4& b) const { return x * b.x + y * b.y + z * b.z + w * b.w; }
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

    // Arithmetic operators
    CVector4 operator+ () const { return *this; }
    CVector4 operator- () const { return CVector4(-x, -y, -z, -w); }
    friend CVector4 operator+ (const CVector4& a, const CVector4& b) { return CVector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
    friend CVector4 operator- (const CVector4& a, const CVector4& b) { return CVector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
    friend CVector4 operator* (const CVector4& a, const T& s) { return CVector4(a.x * s, a.y * s, a.z * s, a.w * s); }
    friend CVector4 operator/ (const CVector4& a, const T& s) { Assert_(s != 0); return CVector4(a.x / s, a.y / s, a.z / s, a.w / s); }

    // Assignment operators
    CVector4& operator+= (const CVector4& b) { *this = *this + b; return *this; }
    CVector4& operator-= (const CVector4& b) { *this = *this - b; return *this; }
    CVector4& operator*= (const T& s) { *this = *this * s; return *this; }
    CVector4& operator/= (const T& s) { *this = *this / s; return *this; }

    // Comparison operators
    friend bool operator== (const CVector4& a, const CVector4& b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
    friend bool operator!= (const CVector4& a, const CVector4& b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }

    // Stream operators
    friend std::ostream& operator<< (std::ostream& stream, const CVector4& a) { stream << "(" << a.x << ", " << a.y << ", " << a.z << ", " << a.w << ")"; return stream; }
};

template <typename T>
CVector4<T> Normalize (const CVector4<T>& a) { CVector4<T> b = a; b.Normalize(); return b; }

using CVector4f = CVector4<CFixed>;
