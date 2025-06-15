#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Shader {
    struct ShaderProgram {
        GLuint handle = 0;
    };

    void uniformSetBool(ShaderProgram& prog, const std::string& name, bool value);
    void uniformSetInt(ShaderProgram& prog, const std::string& name, int value);
    void uniformSetFloat(ShaderProgram& prog, const std::string& name, float value);

    void bindProgramToOpenGL(ShaderProgram prog);
    void unbindProgramFromOpenGL();
    
    void destroyShaderProgram(ShaderProgram& prog);
};
