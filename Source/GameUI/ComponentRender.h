#pragma once

#include "Render/Texture.h"
#include "Render/Timer.h"
#include "Resource/ShaderManager.h"

struct SComponentRender {
    CShaderTag m_shader;
    CTexture m_texture;
    CTimer m_timer;
};