#include "Fixed.h"

#include "CoreDebug.h"
#include <algorithm>
#include <iomanip>
#include <ostream>
#include <sstream>


CFixed CFixed::Sqrt () const {
    static const uint32_t c_maxIter = 10;
    Assert_(m_value >= 0);
    // Newton-Raphson Method
    CFixed next = *this / F_2;
    CFixed result = F_0;
    for (uint32_t i = 0; next != result && i < c_maxIter; ++i) {
        result = next;
        next = (result + *this / result) / F_2;
    }
    return result;
}

std::ostream& operator<< (std::ostream& stream, const CFixed& a) {
    int bits = a.ToBits();
    if (bits < 0) {
        stream << "-";
        bits = -bits;
    }
    stream << (bits >> CFixed::c_fractionBits);
    
    const int fraction = (bits & CFixed::c_fractionMask);
    if (fraction > 0) {
        std::stringstream fractionStream;
        fractionStream << std::setprecision(11) << std::fixed << (double(fraction) / 4096L);
        std::string fractionString = fractionStream.str();
        fractionString.erase(std::find_if(fractionString.rbegin(), fractionString.rend(), [](int ch) {
            return ch != '0';
        }).base(), fractionString.end());
        stream << fractionString.substr(1, fractionString.size() - 1);
    }

    return stream;
}

CFixed Sqrt (CFixed value) {
    return value.Sqrt();
}
