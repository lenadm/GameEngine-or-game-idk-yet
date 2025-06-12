#pragma once

#include <filesystem>
#include <optional>
#include <GLFW/glfw3.h>


namespace Shaders {
    struct IDs {
        GLuint programID = 0;
        GLuint vertexID = 0;
        GLuint fragmentID = 0;
    };

    std::optional<std::string> readFromFile(std::filesystem::path pathToShader);
    GLuint compileShader(std::string shaderSource, GLenum shaderType);

    bool buildProgram(IDs &program);

    void bindProgram(GLuint programID);
    void unbindProgram();

    // void destroy();
};
