#pragma once

#include <ostream>

class CFixed {
public:
    static const unsigned int c_fractionBits = 12;
    static const unsigned int c_fractionMask = 0x00000FFF;
    static const unsigned int c_integerMask = 0xFFFFF000;

    CFixed () = default;
    CFixed (int value) : m_value{value << c_fractionBits} {}

    CFixed Reciprocal () const { return 1 / *this; }
    CFixed Squared () const { return *this * *this; }
    CFixed Sqrt () const;

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

    // Comparison operators
    friend bool operator== (const CFixed& a, const CFixed& b) { return a.m_value == b.m_value; }
    friend bool operator!= (const CFixed& a, const CFixed& b) { return a.m_value != b.m_value; }
    friend bool operator<  (const CFixed& a, const CFixed& b) { return a.m_value < b.m_value; }
    friend bool operator<= (const CFixed& a, const CFixed& b) { return a.m_value <= b.m_value; }
    friend bool operator>  (const CFixed& a, const CFixed& b) { return a.m_value > b.m_value; }
    friend bool operator>= (const CFixed& a, const CFixed& b) { return a.m_value >= b.m_value; }

    // Stream operators
    friend std::ostream& operator<< (std::ostream& stream, const CFixed& a);

    // Bit conversions
    int ToBits () const { return m_value; }
    static CFixed FromBits (int bits) { CFixed f; f.m_value = bits; return f; }

private:
    int m_value{};
};
