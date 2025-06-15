#include <glad/glad.h>
#include <string>
#include <optional>
#include <array>

#include "shader-program.h"

class ShaderBuilder {
    public:
        ShaderBuilder& addShader(const std::string& shaderSource, GLenum shaderType);
        std::optional<ShaderProgram> buildProgram();
        ~ShaderBuilder();

    private:
        struct ShaderEntry {
            GLuint id;
            GLenum type;
        };
        std::array<ShaderEntry, 8> shaders{};
        size_t count = 0;
};
