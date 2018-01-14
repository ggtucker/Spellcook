#include "ShaderManager.h"

CShaderManager::CShaderManager (uint32_t capacity) :
    m_shaderAllocator{capacity} {
}

CShaderTag CShaderManager::Create (const std::string& vertexFileName, const std::string& fragmentFileName) {
    CShaderTag shaderTag = m_shaderAllocator.Create();
    if (CShader* shader = shaderTag.Deref()) {
        shader->Create(vertexFileName, fragmentFileName);
    }
    return shaderTag;
}