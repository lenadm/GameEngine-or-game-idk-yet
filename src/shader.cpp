#include <optional>
#include <string>
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

const std::filesystem::path g_assetsRoot = std::filesystem::current_path() / "assets";

Shader::handle Shader::make(std::string relativePath, GLenum shaderType) {
    Shader::handle newShader = 0;

        std::optional<std::string> shaderSource = Shader::getSourceCode(g_assetsRoot / relativePath);
        if (!shaderSource) return 0;
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
            return 0;
        }
        return newShader;
}

std::optional<std::string> Shader::getSourceCode(std::filesystem::path pathToShader) {
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
