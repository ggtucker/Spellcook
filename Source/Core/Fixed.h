#pragma once

#include <iosfwd>

#define F_2_PI CFixed::FromBits(0x00006488)
#define F_PI CFixed::FromBits(0x00003244)
#define F_HALF_PI CFixed::FromBits(0x00001922)
#define F_QUARTER_PI CFixed::FromBits(0x00000C91)

#define F_0_03125 CFixed::FromBits(0x00000080)
#define F_0_5 CFixed::FromBits(0x00000800)
#define F_1_5 CFixed::FromBits(0x00001800)

#define F_0 CFixed::FromBits(0x00000000)
#define F_1 CFixed::FromBits(0x00001000)
#define F_2 CFixed::FromBits(0x00002000)
#define F_3 CFixed::FromBits(0x00003000)
#define F_4 CFixed::FromBits(0x00004000)

class CFixed {
public:
    static const unsigned int c_fractionBits = 12;
    static const unsigned int c_fractionPrecision = (1 << c_fractionBits);
    static const unsigned int c_fractionMask = 0x00000FFF;
    static const unsigned int c_integerMask = 0xFFFFF000;

    CFixed () = default;
    CFixed (int value) : m_value{value << c_fractionBits} {}
    CFixed (float value) = delete;

    CFixed Reciprocal () const { return F_1 / *this; }
    CFixed Squared () const { return *this * *this; }
    CFixed Sqrt () const;

    // Type conversion
    float ToFloat () const { return static_cast<float>(m_value) / c_fractionPrecision; }
    static CFixed FromFloat (float value) { CFixed f; f.m_value = static_cast<int>(value * c_fractionPrecision); return f; }
    explicit operator float () const { return ToFloat(); }

    // Bit conversions
    int ToBits () const { return m_value; }
    static CFixed FromBits (int bits) { CFixed f; f.m_value = bits; return f; }

    // Arithmetic operators
    CFixed operator+ () const { return *this; }
    CFixed operator- () const { return CFixed::FromBits(-m_value); }
    friend CFixed operator+ (const CFixed& a, const CFixed& b) {
        return FromBits(a.m_value + b.m_value);
    }
    friend CFixed operator- (const CFixed& a, const CFixed& b) {
        return FromBits(a.m_value - b.m_value);
    }
    friend CFixed operator* (const CFixed& a, const CFixed& b) {
        long long int c = static_cast<long long int>(a.m_value) * static_cast<long long int>(b.m_value);
        return FromBits(static_cast<int>(c >> c_fractionBits));
    }
    friend CFixed operator/ (const CFixed& a, const CFixed& b) {
        long long int c = static_cast<long long int>(a.m_value) << c_fractionBits;
        return FromBits(static_cast<int>(c / b.m_value));
    }

    // Assignment operators
    CFixed& operator+= (const CFixed& b) { *this = *this + b; return *this; }
    CFixed& operator-= (const CFixed& b) { *this = *this - b; return *this; }
    CFixed& operator*= (const CFixed& b) { *this = *this * b; return *this; }
    CFixed& operator/= (const CFixed& b) { *this = *this / b; return *this; }

    // Comparison operators
    friend bool operator== (const CFixed& a, const CFixed& b) { return a.m_value == b.m_value; }
    friend bool operator!= (const CFixed& a, const CFixed& b) { return a.m_value != b.m_value; }
    friend bool operator<  (const CFixed& a, const CFixed& b) { return a.m_value < b.m_value; }
    friend bool operator<= (const CFixed& a, const CFixed& b) { return a.m_value <= b.m_value; }
    friend bool operator>  (const CFixed& a, const CFixed& b) { return a.m_value > b.m_value; }
    friend bool operator>= (const CFixed& a, const CFixed& b) { return a.m_value >= b.m_value; }

    // Stream operators
    friend std::ostream& operator<< (std::ostream& stream, const CFixed& a);

private:
    int m_value{};
};

CFixed Sqrt (CFixed value);
