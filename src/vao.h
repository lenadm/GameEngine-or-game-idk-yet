#include <cstddef>
#include <glad/glad.h>
#include <vector>

class VAO {
public:
    VAO();

    struct metaData {
        GLuint bufferIdx;
        size_t length;
        GLenum type;
        bool normalized;
        size_t offset;
    };

    VAO(const VAO&) = delete;
    VAO& operator=(const VAO) = delete;

    VAO(VAO&& other) {
        m_handle = other.m_handle;
        m_bufferAttributes = other.m_bufferAttributes;
        other.m_handle = 0;
        other.m_bufferAttributes = std::vector<metaData>();
    };

    VAO& operator=(VAO&& other) {
        if (this != &other) {
            if (m_handle != 0) {
                glDeleteVertexArrays(1, &m_handle);
            }

            m_bufferAttributes = other.m_bufferAttributes;
            m_handle = other.m_handle;
            other.m_handle = 0;
            other.m_bufferAttributes = std::vector<metaData>();
        }
        return *this;
    }

    GLuint id() const { return m_handle; }
    size_t stride();

    void bindVBO(GLuint VBO);
    void bindEBO(GLuint EBO);

    void addBufferAttributes(GLuint bufferIdx, int length, GLenum type, size_t offset = 0, bool normalized = false);
    void setBufferAttributes();

    void bind();
    void unbind();

    ~VAO();
private:
    GLuint m_handle;
    std::vector<metaData> m_bufferAttributes;
};
