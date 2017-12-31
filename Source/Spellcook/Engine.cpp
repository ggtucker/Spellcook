#include "Engine.h"

#include "Render/ComponentCamera.h"
#include "Game/ComponentPlayer.h"
#include "Render/ComponentRender.h"
#include "Game/ComponentTransform.h"
#include "Game/ComponentVelocity.h"

#include "Render/SingletonCamera.h"
#include "GameUI/SingletonInput.h"

#include "Game/SystemMovement.h"
#include "Game/SystemPlayerController.h"
#include "GameUI/SystemCamera.h"
#include "GameUI/SystemInput.h"
#include "GameUI/SystemRender.h"

#include "GameUI/InputHandler.h"
#include "Render/InputEvent.h"
#include "Render/Timer.h"
#include "Render/Window.h"

void CEngine::RunMainLoop () {
    Initialize();

    const CFixed c_fixedTimestep = F_0_03125;
    const CTime c_timestep = CTime::Seconds(c_fixedTimestep.ToFloat());
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
    m_encosys.RegisterComponent<SComponentCamera>();
    m_encosys.RegisterComponent<SComponentPlayer>();
    m_encosys.RegisterComponent<SComponentRender>();
    m_encosys.RegisterComponent<SComponentTransform>();
    m_encosys.RegisterComponent<SComponentVelocity>();

    // Register singletons
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

    // Manually create the player entity
    ecs::Entity player = m_encosys.Create();
    {
        SComponentTransform& transformComponent = player.AddComponent<SComponentTransform>();
        transformComponent.Position() = math::Vec3f(F_4, F_3, F_2);
        transformComponent.SetForward(math::Vec3f(F_0, F_0, -F_1), math::Vec3f(F_0, F_1, F_0));
        SComponentVelocity& velocityComponent = player.AddComponent<SComponentVelocity>();
        velocityComponent.m_velocity = math::Vec3f(F_1, F_1, F_1);
        SComponentRender& renderComponent = player.AddComponent<SComponentRender>();
        renderComponent.m_shader.Create("..\\Bin\\Shaders\\DefaultShader.vs", "..\\Bin\\Shaders\\DefaultShader.fs");
        renderComponent.m_texture.Create("..\\Bin\\Textures\\Geoff.png");
    }

    // Manually create the camera entity
    ecs::Entity camera = m_encosys.Create();
    {
        SComponentTransform& transformComponent = camera.AddComponent<SComponentTransform>();
        transformComponent.Position() = math::Vec3f(F_0, F_0, -F_3);
        transformComponent.SetForward(math::Vec3f(F_0, F_0, -F_1), math::Vec3f(F_0, F_1, F_0));
        SComponentCamera& cameraComponent = camera.AddComponent<SComponentCamera>();
        cameraComponent.m_followTarget = player.GetId();
        cameraComponent.m_distance = F_0;
        camera.AddComponent<SComponentPlayer>();
    }
    m_encosys.GetSingleton<SSingletonCamera>().m_activeCamera = camera.GetId();
}

void CEngine::Terminate () {

}

void CEngine::Update (CFixed delta) {
    m_encosys.Update(delta);
}
