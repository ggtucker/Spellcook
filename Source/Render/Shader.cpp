#include "Shader.h"

#include "Core/CoreDebug.h"
#include <glad/glad.h>
#include <sstream>
#include <fstream>

DefineLogStatic_(Shader, ELogCategory::Render, "Shader.txt", 0);

static bool ReadFileContents (const std::string& fileName, std::ostringstream& fileContents) {
    std::ifstream file(fileName);
    if (file) {
        fileContents << file.rdbuf();
        file.close();
        return true;
    }
    return false;
}

static bool CompileShader (uint32_t& shaderId, GLenum shaderType, const std::string& fileName) {
    std::ostringstream shaderStream;
    if (!ReadFileContents(fileName, shaderStream)) {
        LogError_(Shader, "Failed to read shader file: ", fileName);
        return false;
    }
    std::string shaderString = shaderStream.str();
    const char* shaderContents = shaderString.c_str();
    uint32_t shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderContents, nullptr);
    glCompileShader(shader);
    int32_t success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char shaderLog[512];
        glGetShaderInfoLog(shader, sizeof(shaderLog), nullptr, shaderLog);
        LogError_(Shader, "Shader compilation failed: ", shaderLog);
        glDeleteShader(shader);
        return false;
    }
    shaderId = shader;
    return true;
}

static bool LinkShaderProgram (uint32_t& programId, uint32_t vertexShader, uint32_t fragmentShader) {
    uint32_t program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    int32_t success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char programLog[512];
        glGetProgramInfoLog(program, sizeof(programLog), nullptr, programLog);
        LogError_(Shader, "Program linking failed: ", programLog);
        glDeleteProgram(program);
        return false;
    }
    programId = program;
    return true;
}

bool CShader::Create (const std::string& vertexFileName, const std::string& fragmentFileName) {
    uint32_t vertexShader;
    if (!CompileShader(vertexShader, GL_VERTEX_SHADER, vertexFileName)) {
        return false;
    }
    uint32_t fragmentShader;
    if (!CompileShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentFileName)) {
        return false;
    }
    uint32_t program;
    if (!LinkShaderProgram(program, vertexShader, fragmentShader)) {
        return false;
    }
    m_program = program;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

void CShader::Use () const {
    glUseProgram(m_program);
}

void CShader::SetBool (const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), (int32_t)value);
}

void CShader::SetInt32 (const std::string& name, int32_t value) const {
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
}

void CShader::SetFloat (const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
}

void CShader::SetVec4 (const std::string& name, const math::Vec4& value) const {
    glUniform4f(
        glGetUniformLocation(m_program, name.c_str()),
        value.x,
        value.y,
        value.z,
        value.w
    );
}

void CShader::SetMat4 (const std::string& name, const math::Mat4& value) const {
    glUniformMatrix4fv(
        glGetUniformLocation(m_program, name.c_str()),
        1,
        false,
        value.ValuePtr()
    );
}
