#pragma once

#include "Core/CoreTypes.h"
#include "Cube.h"

static constexpr uint32_t c_cubeGridLength = 32;
static constexpr uint32_t c_cubeGridHeight = 64;

class CCubeGrid {
public:
    CCubeGrid ();
private:
    CCube m_cubes[c_cubeGridLength][c_cubeGridLength][c_cubeGridHeight];
};