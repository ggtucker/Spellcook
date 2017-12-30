#pragma once

#include <string>
#include "Core/CoreTypes.h"
#include "Math/Matrix4.h"
#include "Math/Vector4.h"

class CShader {
public:
    CShader () = default;

    bool Create (const std::string& vertexFileName, const std::string& fragmentFileName);

    void Use () const;

    void SetBool (const std::string& name, bool value) const;
    void SetInt32 (const std::string& name, int32_t value) const;
    void SetFloat (const std::string& name, float value) const;
    void SetVec4 (const std::string& name, const math::CVector4& value) const;
    void SetMat4 (const std::string& name, const math::CMatrix4& value) const;

private:
    uint32_t m_program{};
};