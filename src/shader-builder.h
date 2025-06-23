#include <glad/glad.h>
#include <string>
#include <optional>

class ShaderBuilder {
    public:
        ShaderBuilder& addShader(const std::string& shaderSource, GLenum shaderType);
        std::optional<GLuint> build();
        ~ShaderBuilder();

    private:
    // Todo: Maybe make this struct public?
        struct ShaderEntry {
            GLuint m_id;
            GLenum m_type;
        };
        ShaderEntry m_shaders[8];
        size_t m_count = 0;
};
