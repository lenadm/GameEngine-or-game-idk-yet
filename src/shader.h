#pragma once

#include <filesystem>
#include <optional>
#include <GLFW/glfw3.h>

namespace Shader {
    typedef unsigned int handle;

    Shader::handle make(std::string relativePath, GLenum shaderType);
    std::optional<std::string> getSourceCode(std::filesystem::path pathToShader);
}
