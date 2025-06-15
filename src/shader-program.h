#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class ShaderProgram {
    public:
        ShaderProgram(GLuint program) : handle(program) {}

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
