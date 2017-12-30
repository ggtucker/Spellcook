#include "Texture.h"

#include "Core/CoreDebug.h"
#include <glad/glad.h>
#include <stb_image.h>

DefineLogStatic_(Texture, ELogCategory::Render, "Texture.txt", 0);

static bool Create2DTexture (uint32_t& textureId, const std::string& textureFileName) {
    int32_t width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    uint8_t* data = stbi_load(textureFileName.c_str(), &width, &height, &channels, 0);
    if (!data) {
        LogError_(Texture, "Failed to read texture file: ", textureFileName);
        return false;
    }
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return true;
}

bool CTexture::Create (const std::string& textureFileName) {
    uint32_t texture;
    if (!Create2DTexture(texture, textureFileName)) {
        return false;
    }
    m_texture = texture;
    return true;
}

void CTexture::Bind (uint32_t slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}