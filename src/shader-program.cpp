#include <optional>
#include <string>
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader-program.h"

const std::filesystem::path g_assetsRoot = std::filesystem::current_path() / "assets";

GLuint Shaders::compileShader(std::string shaderSource, GLenum shaderType) {
    const char* sourcePtr = shaderSource.c_str();
    GLuint newShader = glCreateShader(shaderType);
    glShaderSource(newShader, 1, &sourcePtr, nullptr);

    glCompileShader(newShader);
    int success;
    glGetShaderiv(newShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(newShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
        return 0;
    }

    return newShader;
}

void Shaders::bindProgram(GLuint programID) {
    glUseProgram(programID);
}

void Shaders::unbindProgram() {
    glUseProgram(0);
}

bool Shaders::buildProgram(IDs &program) {
    GLuint programID = glCreateProgram();
    glAttachShader(programID, program.vertexID);
    glAttachShader(programID, program.fragmentID);
    glLinkProgram(programID);
    program.programID = programID;
    return true;
    // TODO: Add linking error handling
}

std::optional<std::string> Shaders::readFromFile(std::filesystem::path pathToShader) {
    pathToShader = g_assetsRoot / pathToShader;

    if (!std::filesystem::exists(pathToShader)) {
        std::cout << "Shader does not exist at this path: " + pathToShader.string() << "\n";
        return std::nullopt;
    } 

    if (!std::filesystem::is_regular_file(pathToShader)) {
        std::cout << "Shader path exists but is not a file: " + pathToShader.string() << "\n";
        return std::nullopt;
    }

    std::ifstream file(pathToShader);
    if (!file) {
        std::cout << "Unable to open shader: " + pathToShader.string() << "\n";
        return std::nullopt;
    }

    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}
