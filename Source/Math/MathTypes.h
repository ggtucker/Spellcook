#pragma once

#include <cmath>

namespace math {

constexpr float PI = 3.1415927f;

constexpr float RadiansToDegrees (float radians) { return radians * 180 / PI; }
constexpr float DegreesToRadians (float degrees) { return degrees * PI / 180; }

float Sqrt (float value);

} // namespace math