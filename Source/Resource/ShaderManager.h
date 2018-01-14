#pragma once

#include "Core/TagAllocator.h"
#include "Render/Shader.h"

using CShaderTag = CTag<CShader>;

class CShaderManager {
public:
    explicit CShaderManager (uint32_t capacity);

    CShaderTag Create (const std::string& vertexFileName, const std::string& fragmentFileName);

private:
    CTagAllocator<CShader> m_shaderAllocator;
};