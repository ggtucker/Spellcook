#include "Engine.h"

#include "Game/SystemMovement.h"
#include "GameUI/InputHandler.h"
#include "GameUI/SystemRender.h"
#include "Render/InputEvent.h"
#include "Render/Timer.h"
#include "Render/Window.h"

void CEngine::RunMainLoop () {
    Initialize();

    const CFixed c_fixedTimestep = F_0_03125;
    const CTime c_timestep = CTime::Seconds(c_fixedTimestep.ToFloat());
    CTime accumulatedTime;
    CTimer engineTimer;

    while (m_window.IsOpen()) {
        CInputEvent event;
        while (m_window.PollInput(event)) {
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
    // Register systems
    m_encosys.RegisterSystem<CSystemMovement>();
    m_encosys.RegisterSystem<CSystemRender>(m_window);

    // Manually create the player entity
    ecs::Entity player = m_encosys.Get(m_encosys.Create());
    SComponentPosition& position = player.AddComponent<SComponentPosition>();
    position.m_position = math::CVector3f(F_4, F_3, F_2);
    SComponentVelocity& velocity = player.AddComponent<SComponentVelocity>();
    velocity.m_velocity = math::CVector3f(F_1, F_1, F_1);
    SComponentRender& render = player.AddComponent<SComponentRender>();
    render.m_shader.Create("..\\Bin\\Shaders\\DefaultShader.vs", "..\\Bin\\Shaders\\DefaultShader.fs");
    render.m_texture.Create("..\\Bin\\Textures\\Geoff.png");
}

void CEngine::Terminate () {

}

void CEngine::ProcessInput (const CInputEvent& event) {
    if (event.m_type == EInputEvent::Closed) {
        m_window.Close();
    }
    else {
        for (CInputHandler* inputHandler : m_inputHandlers) {
            if (inputHandler->ProcessInput(m_window, event)) {
                return;
            }
        }
    }
}

void CEngine::Update (CFixed delta) {
    m_encosys.Update(delta);
}
