#include "vao-builder.h"
#include "opengl-debug.h"

static size_t getSizeOfGLType(GLenum);

VaoBuilder::VaoBuilder() {
    glGenVertexArrays(1, &m_handle);
}

VaoBuilder& VaoBuilder::bindVBO(GLuint VBO) {
    glBindVertexArray(m_handle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(0);
    return *this;
}

VaoBuilder& VaoBuilder::bindEBO(GLuint EBO) {
    glBindVertexArray(m_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindVertexArray(0);
    return *this;
}

VaoBuilder& VaoBuilder::addBufferAttributes(GLuint bufferIdx, int length, GLenum type, size_t offset, bool normalized) {
    RUNTIME_ASSERT(m_count < 16)
    VaoBuilder::metaData md = {};
    md.bufferIdx = bufferIdx;
    md.length = length;
    md.normalized = normalized;
    md.offset = offset;
    md.type = type;
    m_bufferAttributes[m_count] = md;
    m_count++;
    return *this;
}

GLuint VaoBuilder::build() {
    if (m_isAttributesSet) {
        return m_handle;
    }

    glBindVertexArray(m_handle);
    size_t stride = 0;
    for (size_t count = 0; count <= m_count; count++) {
        metaData md = m_bufferAttributes[count];
        stride += (md.length * getSizeOfGLType(md.type)) + md.offset;
    }
    size_t curOffset = 0;
    for (size_t count = 0; count <= m_count; count++) {
        metaData md = m_bufferAttributes[count];
        glVertexAttribPointer(md.bufferIdx, md.length, md.type, md.normalized, stride, (void *) (curOffset + md.offset));
        glEnableVertexAttribArray(md.bufferIdx);
        curOffset += (md.length * getSizeOfGLType(md.type)) + md.offset;
    }
    glBindVertexArray(0);
    m_isAttributesSet = true;
    return m_handle;
}

static size_t getSizeOfGLType(GLenum type)
{
	switch(type)
	{
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			return sizeof(GLbyte);
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
			return sizeof(GLshort);
		case GL_INT_2_10_10_10_REV:
		case GL_INT:
		case GL_UNSIGNED_INT_2_10_10_10_REV:
		case GL_UNSIGNED_INT:
			return sizeof(GLint);
		case GL_FLOAT:
			return sizeof(GLfloat);
		case GL_DOUBLE:
			return sizeof(GLdouble);
		case GL_FIXED:
			return sizeof(GLfixed);
		case GL_HALF_FLOAT:
			return sizeof(GLhalf);
	}

	return 0;
}
