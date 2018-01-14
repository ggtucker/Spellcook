#include "ResourceManager.h"

static const uint32_t c_maxShaderCount = 32;

CResourceManager::CResourceManager () :
    m_shaderManager{c_maxShaderCount} {

}