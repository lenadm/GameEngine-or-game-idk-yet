#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader-program.h"

void ShaderProgram::uniformSetBool(const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(handle, name.c_str()), static_cast<int>(value));
}

void ShaderProgram::uniformSetInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(handle, name.c_str()), value);
}

void ShaderProgram::uniformSetFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(handle, name.c_str()), value);
}

void ShaderProgram::bind() {
    glUseProgram(handle);
}

void ShaderProgram::unbind() {
    glUseProgram(0);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(handle);
}
