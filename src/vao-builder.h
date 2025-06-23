#include <cstddef>
#include <glad/glad.h>
#include <vector>

class VaoBuilder {
public:
    VaoBuilder();

    struct metaData {
        GLuint bufferIdx;
        size_t length;
        GLenum type;
        bool normalized;
        size_t offset;
    };

    VaoBuilder& bindVBO(GLuint VBO);
    VaoBuilder& bindEBO(GLuint EBO);

    VaoBuilder& addBufferAttributes(GLuint bufferIdx, int length, GLenum type, size_t offset = 0, bool normalized = false);
    VaoBuilder& setBufferAttributes();

    GLuint build() const { 
        if (!m_isAttributesSet) return 0;
        return m_handle;
    }

private:
    GLuint m_handle;
    std::vector<metaData> m_bufferAttributes;
    bool m_isAttributesSet = false;
};
