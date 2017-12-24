#pragma once

#include "Core/CoreTypes.h"
#include <string>

class CShader {
public:
    CShader () = default;

    bool Create (const std::string& vertexFileName, const std::string& fragmentFileName);

    void Use ();

    void SetBool (const std::string& name, bool value) const;
    void SetInt32 (const std::string& name, int32_t value) const;
    void SetFloat (const std::string& name, float value) const;

private:
    uint32_t m_program{};
};