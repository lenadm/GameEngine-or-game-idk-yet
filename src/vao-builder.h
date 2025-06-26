#include <cstddef>
#include <glad/glad.h>

class VaoBuilder {
public:
    VaoBuilder();

    struct metaData {
        GLuint bufferIdx;
        size_t length;
        GLenum type;
        size_t offset;
        bool normalized;
    };

    VaoBuilder& bindVBO(GLuint VBO);
    VaoBuilder& bindEBO(GLuint EBO);
    VaoBuilder& addBufferAttributes(GLuint bufferIdx, int length, GLenum type, size_t offset = 0, bool normalized = false);
    GLuint build();

private:
    GLuint m_handle;
    metaData m_bufferAttributes[16] = {};
    size_t m_count = 0;
    bool m_isAttributesSet = false;
};
