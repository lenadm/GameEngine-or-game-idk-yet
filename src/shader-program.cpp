#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader-program.h"

void ShaderProgram::uniformSet1b(const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(m_handle, name.c_str()), static_cast<int>(value));
}

void ShaderProgram::uniformSet1i(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(m_handle, name.c_str()), value);
}

void ShaderProgram::uniformSet1f(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(m_handle, name.c_str()), value);
}

void ShaderProgram::bind() {
    glUseProgram(m_handle);
}

void ShaderProgram::unbind() {
    glUseProgram(0);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_handle);
}
