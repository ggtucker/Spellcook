#pragma once

#include <array>
#include <bitset>
#include <cstdint>
#include "TypeList.h"

#ifndef ENCOSYS_DISABLE_INCLUDE_ECSCONFIG_H
#include "ecsconfig.h"
#endif

#ifndef ENCOSYS_COMPONENTS
#define ENCOSYS_COMPONENTS ecs::tmp::TypeList<>;
#endif

#ifndef ENCOSYS_TIME_TYPE
#define ENCOSYS_TIME_TYPE float
#endif

namespace ecs {

using ComponentTypeId = uint32_t;
using SystemTypeId = uint32_t;
using TimeDelta = ENCOSYS_TIME_TYPE;
using ComponentList = ENCOSYS_COMPONENTS;
using ComponentBitset = std::bitset<ComponentList::Size>;

const uint32_t c_invalidIndex = static_cast<uint32_t>(-1);

}