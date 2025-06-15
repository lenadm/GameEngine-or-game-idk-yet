#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader-program.h"



void Shader::uniformSetBool(Shader::ShaderProgram& prog, const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(prog.handle, name.c_str()), static_cast<int>(value));
}

void Shader::uniformSetInt(Shader::ShaderProgram& prog, const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(prog.handle, name.c_str()), value);
}

void Shader::uniformSetFloat(Shader::ShaderProgram& prog, const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(prog.handle, name.c_str()), value);
}

void Shader::bindProgramToOpenGL(Shader::ShaderProgram prog) {
    glUseProgram(prog.handle);
}

void Shader::unbindProgramFromOpenGL() {
    glUseProgram(0);
}

void Shader::destroyShaderProgram(Shader::ShaderProgram& prog) {
    glDeleteProgram(prog.handle);
}
