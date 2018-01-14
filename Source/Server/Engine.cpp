#include "Engine.h"

#include "Core/Timer.h"

#include "Game/ComponentPlayer.h"
#include "Game/ComponentTransform.h"
#include "Game/ComponentVelocity.h"

#include "Game/SystemMovement.h"
#include "Game/SystemPlayerController.h"

void CEngine::RunMainLoop () {
    Initialize();

    const CFixed c_fixedTimestep = F_0_03125;
    const CTime c_timestep = CTime::Seconds(c_fixedTimestep.ToFloat());
    CTime accumulatedTime;
    CTimer engineTimer;

    while (true) {
        CTime frameTime = engineTimer.Restart();
        accumulatedTime += frameTime;

        while (accumulatedTime >= c_timestep) {
            Update(c_fixedTimestep);
            accumulatedTime -= c_timestep;
        }
    }

    Terminate();
}

void CEngine::Initialize () {
    // Register components
    m_encosys.RegisterComponent<SComponentPlayer>();
    m_encosys.RegisterComponent<SComponentTransform>();
    m_encosys.RegisterComponent<SComponentVelocity>();

    // Register singletons

    // Register systems
    m_encosys.RegisterSystem<CSystemPlayerController>();
    m_encosys.RegisterSystem<CSystemMovement>();

    // Initialize encosys
    m_encosys.Initialize();

    math::Vec3f cubePositions[] = {
        math::Vec3f(0,  0,  0),
        math::Vec3f(2,  5, -15),
        math::Vec3f(-1, -2, -2),
        math::Vec3f(-3, -2, -12),
        math::Vec3f(2, -0, -3),
        math::Vec3f(-1,  3, -7),
        math::Vec3f(1, -2, -2),
        math::Vec3f(1,  2, -2),
        math::Vec3f(1,  0, -1),
        math::Vec3f(-1,  1, -1)
    };

    // Manually create the cube entities
    for (uint32_t i = 0; i < 10; ++i) {
        ecs::Entity cube = m_encosys.Create();
        SComponentTransform& transformComponent = cube.AddComponent<SComponentTransform>();
        transformComponent.Position() = cubePositions[i];
        transformComponent.SetForward(math::Vec3f(F_0, F_0, -F_1), math::Vec3f(F_0, F_1, F_0));
        SComponentVelocity& velocityComponent = cube.AddComponent<SComponentVelocity>();
    }

    // Manually create the player entity
    ecs::Entity player = m_encosys.Create();
    {
        SComponentTransform& transformComponent = player.AddComponent<SComponentTransform>();
        transformComponent.Position() = math::Vec3f(F_0, F_0, -F_3);
        transformComponent.SetForward(math::Vec3f(F_0, F_0, -F_1), math::Vec3f(F_0, F_1, F_0));
        player.AddComponent<SComponentPlayer>();
    }
}

void CEngine::Terminate () {

}

void CEngine::Update (CFixed delta) {
    m_encosys.Update(delta);
}
