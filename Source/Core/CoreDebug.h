#pragma once

#include <cassert>

#ifdef DEBUG
#define Assert_(x) assert(x)
#else
#define Assert_(x)
#endif