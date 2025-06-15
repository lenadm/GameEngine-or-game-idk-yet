#include <iostream>
#include <glad/glad.h>

#include "shader-program.h"
#include "shader-builder.h"

static GLuint compileShader(const std::string& shaderSource, const GLenum shaderType);

ShaderBuilder& ShaderBuilder::addShader(
    const std::string& shaderSource,
    const GLenum shaderType
) {
    if (count >= 8) {
        std::cout << "Warning: tried adding >8 shaders to builder\n";
        return *this;
    }

    shaders[count].id = compileShader(shaderSource, shaderType);
    shaders[count].type = shaderType;
    count++;
    return *this;
}

std::optional<ShaderProgram> ShaderBuilder::buildProgram() {
    GLuint shaderProgram = glCreateProgram();
    for (size_t i = 0; i <= count; i++) {
        glAttachShader(shaderProgram, shaders[i].id);
    }
    glLinkProgram(shaderProgram);
    GLint linkStatus; 
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
    if ((GLenum) linkStatus == GL_TRUE) {
        return ShaderProgram(shaderProgram);
    }
        return std::nullopt;
}

ShaderBuilder::~ShaderBuilder() {
    for (size_t i = 0; i < count; i++) {
        glDeleteShader(shaders[i].id);
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
