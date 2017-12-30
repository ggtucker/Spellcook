#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Timer.h"

struct SComponentRender {
    CShader m_shader;
    CTexture m_texture;
    CTimer m_timer;
};