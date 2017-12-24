#pragma once

#include "Core/CoreDebug.h"
#include "Core/CoreTypes.h"
#include <ostream>

template <typename T>
class CVector3 {
public:
    T x, y, z;

    CVector3 () : x{}, y{}, z{} {}
    CVector3 (const T& x, const T& y) : x{x}, y{y}, z{} {}
    CVector3 (const T& x, const T& y, const T& z) : x{x}, y{y}, z{z} {}

    T Dot (const CVector3& b) const { return x * b.x + y * b.y + z * b.z; }
    T Cross (const CVector3& b) const { return CVector3(y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
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

    // Arithmetic operators
    CVector3 operator+ () const { return *this; }
    CVector3 operator- () const { return CVector3(-x, -y, -z); }
    friend CVector3 operator+ (const CVector3& a, const CVector3& b) { return CVector3(a.x + b.x, a.y + b.y, a.z + b.z); }
    friend CVector3 operator- (const CVector3& a, const CVector3& b) { return CVector3(a.x - b.x, a.y - b.y, a.z - b.z); }
    friend CVector3 operator* (const CVector3& a, const T& s) { return CVector3(a.x * s, a.y * s, a.z * s); }
    friend CVector3 operator/ (const CVector3& a, const T& s) { Assert_(s != 0); return CVector3(a.x / s, a.y / s, a.z / s); }

    // Assignment operators
    CVector3& operator+= (const CVector3& b) { *this = *this + b; return *this; }
    CVector3& operator-= (const CVector3& b) { *this = *this - b; return *this; }
    CVector3& operator*= (const T& s) { *this = *this * s; return *this; }
    CVector3& operator/= (const T& s) { *this = *this / s; return *this; }

    // Comparison operators
    friend bool operator== (const CVector3& a, const CVector3& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
    friend bool operator!= (const CVector3& a, const CVector3& b) { return a.x != b.x || a.y != b.y || a.z != b.z; }

    // Stream operators
    friend std::ostream& operator<< (std::ostream& stream, const CVector3& a) { stream << "(" << a.x << ", " << a.y << ", " << a.z << ")"; return stream; }
};

template <typename T>
CVector3<T> Normalize (const CVector3<T>& a) { CVector3<T> b = a; b.Normalize(); return b; }

using CVector3f = CVector3<CFixed>;
