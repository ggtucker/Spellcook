#include "Engine.h"

#include "Render/Window.h"
#include "Game/SystemMovement.h"
#include "GameUI/SystemRender.h"

void CEngine::Initialize (CWindow& window) {
    // Register systems
    m_encosys.RegisterSystem<CSystemMovement>();
    m_encosys.RegisterSystem<CSystemRender>(window);

    // Manually create the player entity
    ecs::Entity player = m_encosys.Get(m_encosys.Create());
    SComponentPosition& position = player.AddComponent<SComponentPosition>();
    position.m_position = CVector3f(F_4, F_3, F_2);
    SComponentVelocity& velocity = player.AddComponent<SComponentVelocity>();
    velocity.m_velocity = CVector3f(F_1, F_1, F_1);
    SComponentRender& render = player.AddComponent<SComponentRender>();
    render.m_shader.Create("..\\Bin\\Shaders\\DefaultShader.vs", "..\\Bin\\Shaders\\DefaultShader.fs");
}

void CEngine::Terminate () {

}

void CEngine::ProcessInput (CWindow& window, const CInputEvent& event) {
    if (event.m_type == EInputEvent::Closed) {
        window.Close();
    }
    else {
        for (CInputHandler* inputHandler : m_inputHandlers) {
            if (inputHandler->ProcessInput(window, event)) {
                return;
            }
        }
    }
}

void CEngine::Update (CFixed delta) {
    m_encosys.Update(delta);
}
