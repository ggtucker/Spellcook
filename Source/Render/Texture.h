#pragma once

#include <string>

class CTexture {
public:
    CTexture () = default;

    bool Create (const std::string& textureFileName);

    void Bind (uint32_t slot = 0);

private:
    uint32_t m_texture;
};