#pragma once

#include "Core/Timer.h"
#include "Render/Texture.h"
#include "Resource/ShaderManager.h"

struct SComponentRender {
    CShaderTag m_shader;
    CTexture m_texture;
    CTimer m_timer;
};