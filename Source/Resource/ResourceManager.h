#pragma once

#include "ShaderManager.h"

class CResourceManager {
public:
    CResourceManager ();

    CShaderManager& Shader () { return m_shaderManager; }
    const CShaderManager& Shader () const { return m_shaderManager; }

private:
    CShaderManager m_shaderManager;
};