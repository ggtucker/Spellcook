#pragma once

#include "Trig.h"
#include "Vector3.h"
#include "Vector4.h"

namespace math {

template <typename T>
class CMatrix4T {
public:
    CMatrix4T () { Reset(); }

    CMatrix4T (
        const T& c00, const T& c10, const T& c20, const T& c30,
        const T& c01, const T& c11, const T& c21, const T& c31,
        const T& c02, const T& c12, const T& c22, const T& c32,
        const T& c03, const T& c13, const T& c23, const T& c33
    ) {
        col[0][0] = c00; col[1][0] = c10; col[2][0] = c20; col[3][0] = c30;
        col[0][1] = c01; col[1][1] = c11; col[2][1] = c21; col[3][1] = c31;
        col[0][2] = c02; col[1][2] = c12; col[2][2] = c22; col[3][2] = c32;
        col[0][3] = c03; col[1][3] = c13; col[2][3] = c23; col[3][3] = c33;
    }

    CMatrix4T (
        const CVector4T<T>& c0,
        const CVector4T<T>& c1,
        const CVector4T<T>& c2,
        const CVector4T<T>& c3
    ) {
        col[0] = c0; col[1] = c1; col[2] = c2; col[3] = c3;
    }

    template <typename U>
    CMatrix4T<U> Cast () const {
        return CMatrix4T<U>{
            col[0].Cast<U>(),
            col[1].Cast<U>(),
            col[2].Cast<U>(),
            col[3].Cast<U>()
        };
    }

    void Reset () {
        col[0][0] = static_cast<T>(1); col[1][0] = static_cast<T>(0); col[2][0] = static_cast<T>(0); col[3][0] = static_cast<T>(0);
        col[0][1] = static_cast<T>(0); col[1][1] = static_cast<T>(1); col[2][1] = static_cast<T>(0); col[3][1] = static_cast<T>(0);
        col[0][2] = static_cast<T>(0); col[1][2] = static_cast<T>(0); col[2][2] = static_cast<T>(1); col[3][2] = static_cast<T>(0);
        col[0][3] = static_cast<T>(0); col[1][3] = static_cast<T>(0); col[2][3] = static_cast<T>(0); col[3][3] = static_cast<T>(1);
    }

    const T* ValuePtr () const { return &col[0][0]; }
    CVector4T<T>& operator[] (uint32_t i) { return col[i]; }
    const CVector4T<T>& operator[] (uint32_t i) const { return col[i]; }

    friend CMatrix4T operator* (const CMatrix4T& a, const CMatrix4T& b) {
        return CMatrix4T<T>{
            CVector4T<T>{
                b[0].Dot({ a[0][0], a[1][0], a[2][0], a[3][0] }),
                b[0].Dot({ a[0][1], a[1][1], a[2][1], a[3][1] }),
                b[0].Dot({ a[0][2], a[1][2], a[2][2], a[3][2] }),
                b[0].Dot({ a[0][3], a[1][3], a[2][3], a[3][3] })
            },
            CVector4T<T> {
                b[1].Dot({ a[0][0], a[1][0], a[2][0], a[3][0] }),
                b[1].Dot({ a[0][1], a[1][1], a[2][1], a[3][1] }),
                b[1].Dot({ a[0][2], a[1][2], a[2][2], a[3][2] }),
                b[1].Dot({ a[0][3], a[1][3], a[2][3], a[3][3] })
            },
            CVector4T<T> {
                b[2].Dot({ a[0][0], a[1][0], a[2][0], a[3][0] }),
                b[2].Dot({ a[0][1], a[1][1], a[2][1], a[3][1] }),
                b[2].Dot({ a[0][2], a[1][2], a[2][2], a[3][2] }),
                b[2].Dot({ a[0][3], a[1][3], a[2][3], a[3][3] })
            },
            CVector4T<T> {
                b[3].Dot({ a[0][0], a[1][0], a[2][0], a[3][0] }),
                b[3].Dot({ a[0][1], a[1][1], a[2][1], a[3][1] }),
                b[3].Dot({ a[0][2], a[1][2], a[2][2], a[3][2] }),
                b[3].Dot({ a[0][3], a[1][3], a[2][3], a[3][3] })
            }
        };
    }

    friend CVector4T<T> operator* (const CMatrix4T& a, const CVector4T<T>& v) {
        return CVector4T<T>{
            v.Dot({ a[0][0], a[1][0], a[2][0], a[3][0] }),
            v.Dot({ a[0][1], a[1][1], a[2][1], a[3][1] }),
            v.Dot({ a[0][2], a[1][2], a[2][2], a[3][2] }),
            v.Dot({ a[0][3], a[1][3], a[2][3], a[3][3] })
        };
    }

private:
    CVector4T<T> col[4];
};

template <typename T>
CMatrix4T<T> Translate (const CMatrix4T<T>& m, const CVector3T<T>& t) {
    return CMatrix4T<T>{
        m[0][0], m[1][0], m[2][0], t.x + m[3][0],
        m[0][1], m[1][1], m[2][1], t.y + m[3][1],
        m[0][2], m[1][2], m[2][2], t.z + m[3][2],
        m[0][3], m[1][3], m[2][3], m[3][3]
    };
}

template <typename T>
CMatrix4T<T> Scale (const CMatrix4T<T>& m, const CVector3T<T>& s) {
    return CMatrix4T<T>{
        s.x * m[0][0], m[1][0],       m[2][0],       m[3][0],
        m[0][1],       s.y * m[1][1], m[2][1],       m[3][1],
        m[0][2],       m[1][2],       s.z * m[2][2], m[3][2],
        m[0][3],       m[1][3],       m[2][3],       m[3][3]
    };
}

template <typename T>
CMatrix4T<T> Scale (const CMatrix4T<T>& m, const T& s) {
    return CMatrix4T<T>{
        s * m[0][0], m[1][0],     m[2][0],     m[3][0],
        m[0][1],     s * m[1][1], m[2][1],     m[3][1],
        m[0][2],     m[1][2],     s * m[2][2], m[3][2],
        m[0][3],     m[1][3],     m[2][3],     m[3][3]
    };
}

template <typename T>
CMatrix4T<T> Rotate (const CMatrix4T<T>& m, const CVector3T<T>& a, const T& angle) {
    const T c = Cos(angle);
    const T oc = 1 - c;
    const T s = Sin(angle);
    const T os = 1 - s;
    return m * CMatrix4T<T>{
        a.x*a.x*oc + c,     a.x*a.y*oc - a.z*s, a.x*a.z*oc + a.y*s, static_cast<T>(0),
        a.y*a.x*oc + a.z*s, a.y*a.y*oc + c,     a.y*a.z*oc - a.x*s, static_cast<T>(0),
        a.z*a.x*oc - a.y*s, a.z*a.y*oc + a.x*s, a.z*a.z*oc + c,     static_cast<T>(0),
        static_cast<T>(0),  static_cast<T>(0),  static_cast<T>(0),  static_cast<T>(1)
    };
}

template <typename T>
CMatrix4T<T> Perspective (const T& fov, const T& aspect, const T& near, const T& far) {
    const T tanHalfFov = Tan(fov / static_cast<T>(2));
    return CMatrix4T<T>{
        CVector4T<T>{
            static_cast<T>(1) / (aspect * tanHalfFov),
            static_cast<T>(0),
            static_cast<T>(0),
            static_cast<T>(0)
        },
        CVector4T<T> {
            static_cast<T>(0),
            static_cast<T>(1) / tanHalfFov,
            static_cast<T>(0),
            static_cast<T>(0)
        },
        CVector4T<T> {
            static_cast<T>(0),
            static_cast<T>(0),
            -(far + near) / (far - near),
            -static_cast<T>(1)
        },
        CVector4T<T> {
            static_cast<T>(0),
            static_cast<T>(0),
            -(static_cast<T>(2) * far * near) / (far - near),
            static_cast<T>(0)
        }
    };
}

template <typename T>
CMatrix4T<T> LookAt (const CVector3T<T>& eye, const CVector3T<T>& target, const CVector3T<T>& up) {
    const CVector3T<T> f = Normalize(target - eye);
    const CVector3T<T> r = Normalize(Cross(f, up));
    const CVector3T<T> u = Cross(r, f);
    return CMatrix4T<T>{
        r.x,               r.y,               r.z,               -Dot(r, eye),
        u.x,               u.y,               u.z,               -Dot(u, eye),
        f.x,               f.y,               f.z,               -Dot(f, eye),
        static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)
    };
}

using Mat4 = CMatrix4T<float>;
using Mat4f = CMatrix4T<CFixed>;

} // namespace math