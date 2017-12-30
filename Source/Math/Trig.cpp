#include "Trig.h"

#include <cmath>
#include <cstdint>

namespace math {

CFixed Sin (CFixed value) {
    // S(x) = (1/2)z * (C1 - z^2[C2 - z^2(C3)])
    // x  : int32 contained in value (value.m_value)
    // z  : x        / 2^qI  (or: x        >> qI)
    // C1 : (pi)     * 2^qN  (or: (pi)     << qN)
    // C2 : (2*pi-5) * 2^qN  (or: (2*pi-5) << qN)
    // C3 : (pi-3)   * 2^qN  (or: (pi-3)   << qN)
    // qI : Q-pos for input (quarter circle)  12
    // qO : Q-pos for output                  12
    // qN : Q-pos for intermediate            30
    static const int64_t C1 = 3373259426; // Q30
    static const int64_t C2 = 1377809732; // Q30
    static const int64_t C3 = 152033954;  // Q30
    static const int64_t qI = 12;
    static const int64_t qO = 12;
    static const int64_t BitCount = sizeof(int64_t) * 8;

    int64_t x, x2, y;

    // shift Q12 -> Q62
    x = static_cast<int64_t>(value.ToBits()) << ((BitCount - 2) - qI);

    // test for quadrant 1 or 2
    if ((x ^ (x << 1)) < 0) {
        x = (1ll << (BitCount - 1)) - x;
    }

    // shift Q62 -> Q12
    x = x >> ((BitCount - 2) - qI);

    x2 = x * x;                // Q24
    y = C3 * x2 >> 24;         // Q30
    y = C2 - y;                // Q30
    y = y * x2 >> 24;          // Q30
    y = C1 - y;                // Q30
    y = y * x;                 // Q42
    y = y >> (42 - qO + 1);    // Q12

    return CFixed::FromBits(static_cast<int32_t>(y));
}

CFixed Cos (CFixed value) {
    return Sin(F_1 - value);
}

CFixed Tan (CFixed value) {
    return Sin(value) / Cos(value);
}

float Sin (float value) {
    return sin(value);
}

float Cos (float value) {
    return cos(value);
}

float Tan (float value) {
    return tan(value);
}

} // namespace math