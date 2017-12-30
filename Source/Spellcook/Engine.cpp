#include "Engine.h"

#include "Game/ComponentPosition.h"
#include "Game/ComponentVelocity.h"
#include "Render/ComponentRender.h"

#include "Game/SystemMovement.h"
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
        CInputEvent event;
        while (g_window.PollInput(event)) {
            ProcessInput(event);
        }

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
    m_encosys.RegisterComponent<SComponentPosition>();
    m_encosys.RegisterComponent<SComponentVelocity>();
    m_encosys.RegisterComponent<SComponentRender>();

    // Register systems
    m_encosys.RegisterSystem<CSystemMovement>();
    m_encosys.RegisterSystem<CSystemRender>();

    // Initialize encosys
    m_encosys.Initialize();

    // Manually create the player entity
    ecs::EntityId player = m_encosys.Create();
    SComponentPosition& position = m_encosys.AddComponent<SComponentPosition>(player);
    position.m_position = math::Vec3f(F_4, F_3, F_2);
    SComponentVelocity& velocity = m_encosys.AddComponent<SComponentVelocity>(player);
    velocity.m_velocity = math::Vec3f(F_1, F_1, F_1);
    SComponentRender& render = m_encosys.AddComponent<SComponentRender>(player);
    render.m_shader.Create("..\\Bin\\Shaders\\DefaultShader.vs", "..\\Bin\\Shaders\\DefaultShader.fs");
    render.m_texture.Create("..\\Bin\\Textures\\Geoff.png");
}

void CEngine::Terminate () {

}

void CEngine::ProcessInput (const CInputEvent& event) {
    if (event.m_type == EInputEvent::Closed) {
        g_window.Close();
    }
    else {
        for (CInputHandler* inputHandler : m_inputHandlers) {
            if (inputHandler->ProcessInput(g_window, event)) {
                return;
            }
        }
    }
}

void CEngine::Update (CFixed delta) {
    m_encosys.Update(delta.ToFloat());
}
