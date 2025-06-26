#include <glad/glad.h>
#include <string>

class ShaderBuilder {
    public:
        ShaderBuilder& addShader(const std::string& shaderSource, GLenum shaderType);
        ShaderBuilder& addShaderFromPath(const std::string& pathToShader, GLenum shaderType);
        GLuint build();
        ~ShaderBuilder();

        struct ShaderEntry {
            GLuint m_id;
            GLenum m_type;
        };
    private:
        ShaderEntry m_shaders[8];
        size_t m_count = 0;
};
