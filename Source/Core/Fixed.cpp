#include "Fixed.h"
#include <iomanip>
#include <sstream>

CFixed CFixed::Sqrt () const {
    // Pick starting value
    CFixed next = 1;

    // Newton-Raphson Method
    CFixed result = 0;
    do {
        result = next;
        next = result - (result.Squared() - *this) / (2 * result);
    } while (next != result);
    return result;
}

std::ostream& operator<< (std::ostream& stream, const CFixed& a) {
    const int bits = a.ToBits();
    stream << (bits >> CFixed::c_fractionBits);
    
    const int fraction = (bits & CFixed::c_fractionMask);
    if (fraction > 0) {
        std::stringstream fractionStream;
        fractionStream << std::setprecision(11) << std::fixed << (double(fraction) / 4096L);
        const std::string fractionString = fractionStream.str();
        stream << fractionString.substr(1, fractionString.size() - 1);
    }

    return stream;
}