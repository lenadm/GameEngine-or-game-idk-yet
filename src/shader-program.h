#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class ShaderProgram {
public:
    ShaderProgram() = delete;
    ShaderProgram(GLuint program) : handle(program) {}

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram) = delete;

    ShaderProgram(ShaderProgram&& other) : handle(other.handle) {
        other.handle = 0;
    };

    ShaderProgram& operator=(ShaderProgram&& other) {
        if (this != &other) {
            if (handle != 0) {
                glDeleteProgram(handle);
            }

            handle = other.handle;
            other.handle = 0;
        }
        return *this;
    }

    GLuint id() const { return handle; }

    void uniformSetBool(const std::string& name, bool value);
    void uniformSetInt(const std::string& name, int value);
    void uniformSetFloat(const std::string& name, float value);

    void bind();
    void unbind();

    ~ShaderProgram();
private:
    GLuint handle = 0;
};
