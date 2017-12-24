#pragma once

#include "Core/Fixed.h"
#include "Game/ComponentPosition.h"
#include "Game/ComponentVelocity.h"
#include "Render/ComponentRender.h"

using EncosysComponents = ecs::tmp::TypeList<
    SComponentPosition,
    SComponentRender,
    SComponentVelocity
>;

#define ENCOSYS_COMPONENTS EncosysComponents

#define ENCOSYS_TIME_TYPE CFixed
