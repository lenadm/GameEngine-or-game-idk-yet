#include <optional>
#include <string>
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader-program.h"

const std::filesystem::path g_assetsRoot = std::filesystem::current_path() / "assets";

ShaderProgram::ShaderProgram() { }

bool ShaderProgram::addShader(std::string relativePath, GLenum shaderType) {
    handle newShader = 0;


    std::optional<std::string> shaderSource = readFromFile(g_assetsRoot / relativePath);
    if (!shaderSource) {
        std::string shader;
        switch (shaderType) {
            case GL_FRAGMENT_SHADER: { shader = "fragment"; break; }
            case GL_VERTEX_SHADER:   { shader = "vertex";   break; }
            default:                 { shader = "unknown";  break; }
        }
        std::cout << "Error occured whilst trying to compile: " << shader << "shader" << "\n";
        return false;
    }
    const char* sourcePtr = shaderSource.value().c_str();

    newShader = glCreateShader(shaderType);
    glShaderSource(newShader, 1, &sourcePtr, nullptr);

    glCompileShader(newShader);
    int success;
    glGetShaderiv(newShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(newShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
        return false;
    }

    switch (shaderType) {
        case GL_VERTEX_SHADER: { this->vertexID_ = newShader; break; }
        case GL_FRAGMENT_SHADER: { this->fragmentID_ = newShader; break; }
        default: { std::cout << "Unkown shaderType, skipping: " << shaderType; break;}
    }
    return true;
}

void ShaderProgram::bind() {
    glUseProgram(programID_);
}

void ShaderProgram::unbind() {
    glUseProgram(0);
}

bool ShaderProgram::build() {
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexID_);
    glAttachShader(shaderProgram, fragmentID_);
    glLinkProgram(shaderProgram);
    programID_ = shaderProgram;
    glDeleteShader(vertexID_);
    glDeleteShader(fragmentID_);
    return true;
    // TODO: Add linking error handling
}

std::optional<std::string> ShaderProgram::readFromFile(std::filesystem::path pathToShader) {
    if (!std::filesystem::exists(pathToShader)) {
        std::cout << "Shader does not exist at this path: " + pathToShader.string();
        return std::nullopt;
    } 

    if (!std::filesystem::is_regular_file(pathToShader)) {
        std::cout << "Shader path exists but is not a file: " + pathToShader.string();
        return std::nullopt;
    }

    std::ifstream file(pathToShader);
    if (!file) {
        std::cout << "Unable to open shader: " + pathToShader.string();
        return std::nullopt;
    }

    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}
