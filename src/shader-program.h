#pragma once

#include <filesystem>
#include <optional>
#include <GLFW/glfw3.h>

typedef unsigned int handle;

class ShaderProgram {
    public:
        ShaderProgram();

        bool addShader(std::string relativePath, GLenum shaderType);
        
        bool build();
        
        void bind();
        void unbind();

        GLuint id() const { return programID_; };
    private:
        GLuint programID_ = 0;
        GLuint vertexID_ = 0;
        GLuint fragmentID_ = 0;
        
        // Disallow copying
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        // Allow moving
        ShaderProgram(ShaderProgram&& other) noexcept;
        ShaderProgram& operator=(ShaderProgram&& other) noexcept;

        std::optional<std::string> readFromFile(std::filesystem::path pathToShader);
};
