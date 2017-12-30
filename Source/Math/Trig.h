#pragma once

#include "Core/Fixed.h"

namespace math {

// value is in normalized units where 1 is equivalent to pi/2 radians
CFixed Sin (CFixed value);

// value is in normalized units where 1 is equivalent to pi/2 radians
CFixed Cos (CFixed value);

// value is in radians
float Sin (float value);

// value is in radians
float Cos (float value);

} // namespace math