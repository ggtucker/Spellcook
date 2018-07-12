#include "Engine.h"

#include "Core/Timer.h"

#include "Game/ComponentPlayer.h"
#include "GameRender/ComponentRender.h"
#include "Game/ComponentTransform.h"
#include "Game/ComponentVelocity.h"

#include "Game/SingletonWorld.h"
#include "GameRender/SingletonCamera.h"
#include "GameRender/SingletonInput.h"

#include "Game/SystemMovement.h"
#include "Game/SystemPlayerController.h"
#include "GameRender/SystemCamera.h"
#include "GameRender/SystemInput.h"
#include "GameRender/SystemRender.h"

#include "GameRender/InputHandler.h"
#include "Render/InputEvent.h"
#include "Render/Window.h"

void CEngine::RunMainLoop () {
    Initialize();

    const float c_fixedTimestep = 0.03125f;
    const CTime c_timestep = CTime::Seconds(c_fixedTimestep);
    CTime accumulatedTime;
    CTimer engineTimer;

    while (g_window.IsOpen()) {
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
    m_encosys.RegisterComponent<SComponentRender>();
    m_encosys.RegisterComponent<SComponentTransform>();
    m_encosys.RegisterComponent<SComponentVelocity>();

    // Register singletons
    m_encosys.RegisterSingleton<SSingletonWorld>();
    m_encosys.RegisterSingleton<SSingletonCamera>();
    m_encosys.RegisterSingleton<SSingletonInput>();

    // Register systems
    m_encosys.RegisterSystem<CSystemInput>();
    m_encosys.RegisterSystem<CSystemPlayerController>();
    m_encosys.RegisterSystem<CSystemMovement>();
    m_encosys.RegisterSystem<CSystemCamera>();
    m_encosys.RegisterSystem<CSystemRender>();

    // Initialize encosys
    m_encosys.Initialize();

    math::Vec3 cubePositions[] = {
        math::Vec3(0,  0,  0),
        math::Vec3(2,  5, -15),
        math::Vec3(-1, -2, -2),
        math::Vec3(-3, -2, -12),
        math::Vec3(2, -0, -3),
        math::Vec3(-1,  3, -7),
        math::Vec3(1, -2, -2),
        math::Vec3(1,  2, -2),
        math::Vec3(1,  0, -1),
        math::Vec3(-1,  1, -1)
    };

    CShaderTag shaderTag = m_resource.Shader().Create("..\\Bin\\Shaders\\DefaultShader.vs", "..\\Bin\\Shaders\\DefaultShader.fs");

    // Manually create the cube entities
    for (uint32_t i = 0; i < 10; ++i) {
        ecs::Entity cube = m_encosys.Create();
        SComponentTransform& transformComponent = cube.AddComponent<SComponentTransform>();
        transformComponent.Position() = cubePositions[i];
        transformComponent.SetForward(math::Vec3(0.f, 0.f, -1.f), math::Vec3(0.f, 1.f, 0.f));
        SComponentVelocity& velocityComponent = cube.AddComponent<SComponentVelocity>();
        SComponentRender& renderComponent = cube.AddComponent<SComponentRender>();
        renderComponent.m_shader = shaderTag;
        renderComponent.m_texture.Create("..\\Bin\\Textures\\Geoff.png");
    }

    // Manually create the player entity
    ecs::Entity player = m_encosys.Create();
    {
        SComponentTransform& transformComponent = player.AddComponent<SComponentTransform>();
        transformComponent.Position() = math::Vec3(0.f, 0.f, -3.f);
        transformComponent.SetForward(math::Vec3(0.f, 0.f, -1.f), math::Vec3(0.f, 1.f, 0.f));
        player.AddComponent<SComponentPlayer>();
    }
    m_encosys.GetSingleton<SSingletonCamera>().m_cameraTarget = player.GetId();
}

void CEngine::Terminate () {

}

void CEngine::Update (float delta) {
    m_encosys.Update(delta);
}
