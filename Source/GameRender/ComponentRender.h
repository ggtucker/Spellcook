#pragma once

#include "Render/Texture.h"
#include "Render/Mesh.h"
#include "Resource/ShaderManager.h"

enum class ERenderType {
    Mesh
};

struct SComponentRender {
    ERenderType m_type;
    CShaderTag m_shader;
    CTexture m_texture;
    CMesh m_mesh;
};