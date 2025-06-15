#include <glad/glad.h>
#include <string>
#include <optional>

#include "shader-program.h"

class ShaderBuilder {
    public:
        ShaderBuilder& addShader(const std::string& shaderSource, GLenum shaderType);
        std::optional<Shader::ShaderProgram> buildProgram();
        ~ShaderBuilder();

    private:
        GLuint IDs[8] = {0};
        GLenum Types[8] = {0};
        size_t count = 0;
};
