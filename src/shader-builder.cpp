#include <iostream>
#include <glad/glad.h>
#include <filesystem>
#include <fstream>

#include "shader-builder.h"
#include "opengl-debug.h"

static GLuint compileShader(const std::string& shaderSource, const GLenum shaderType);

ShaderBuilder& ShaderBuilder::addShader(
    const std::string& shaderSource,
    const GLenum shaderType
) {
    RUNTIME_ASSERT(m_count < 8)

    m_shaders[m_count].m_id = compileShader(shaderSource, shaderType);
    m_shaders[m_count].m_type = shaderType;
    m_count++;
    return *this;
}

ShaderBuilder& ShaderBuilder::addShaderFromPath(const std::string& path, GLenum shaderType) {
    RUNTIME_ASSERT(std::filesystem::exists(path))
    RUNTIME_ASSERT(std::filesystem::is_regular_file(path))

    std::ifstream file(path);
    if (!file) {
        std::cout << "Unable to open shader: " + path << "\n";
        return *this;
    }

    auto outBuf = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    addShader(outBuf, shaderType);
    return *this;
}


GLuint ShaderBuilder::build() {
    GLuint shaderProgram = glCreateProgram();
    for (size_t i = 0; i <= m_count; i++) {
        glAttachShader(shaderProgram, m_shaders[i].m_id);
    }
    glLinkProgram(shaderProgram);
    GLint linkStatus; 
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
    if ((GLenum) linkStatus == GL_TRUE) {
        return shaderProgram;
    }
    return 0;
}

ShaderBuilder::~ShaderBuilder() {
    for (size_t i = 0; i < m_count; i++) {
        glDeleteShader(m_shaders[i].m_id);
    }
}

static GLuint compileShader(const std::string& shaderSource, const GLenum shaderType) {
    const char* sourcePtr = shaderSource.c_str();
    GLuint newShader = glCreateShader(shaderType);
    glShaderSource(newShader, 1, &sourcePtr, nullptr);

    glCompileShader(newShader);
    int success;
    glGetShaderiv(newShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(newShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << "\n" << shaderSource << "\n";
        return 0;
    }

    return newShader;
}
