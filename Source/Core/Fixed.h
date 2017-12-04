#pragma once

class CFixed {
public:
    static const int c_fractionBits = 12;

    CFixed () = default;
    CFixed (int value) : m_value{value << c_fractionBits} {}

    CFixed Sqrt () const;

    // Arithmetic operators

    // Comparison operators

    // Bit conversions

private:
    int m_value{};
};