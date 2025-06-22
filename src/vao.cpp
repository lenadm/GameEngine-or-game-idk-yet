#include "vao.h"
#include <iostream>

static size_t getSizeOfGLType(GLenum type);

VAO::VAO() {
    glGenVertexArrays(1, &m_handle);
}

size_t VAO::stride() {
    size_t stride = 0;
    for (VAO::metaData md : m_bufferAttributes) {
        stride += (md.length * getSizeOfGLType(md.type)) + md.offset;
    }
    return stride;
}

void VAO::bindVBO(GLuint VBO) {
    glBindVertexArray(m_handle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(0);
}

void VAO::bindEBO(GLuint EBO) {
    glBindVertexArray(m_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindVertexArray(0);
}

void VAO::addBufferAttributes(GLuint bufferIdx, int length, GLenum type, size_t offset, bool normalized) {
    VAO::metaData md = {};
    md.bufferIdx = bufferIdx;
    md.length = length;
    md.normalized = normalized;
    md.offset = offset;
    md.type = type;
    m_bufferAttributes.push_back(md);
}

void VAO::setBufferAttributes() {
    glBindVertexArray(m_handle);
    size_t stride = this->stride();
    size_t curOffset = 0;
    for (metaData md : m_bufferAttributes) {
        glVertexAttribPointer(md.bufferIdx, md.length, md.type, md.normalized, stride, (void *) (curOffset + md.offset));
        glEnableVertexAttribArray(md.bufferIdx);
        curOffset += (md.length * getSizeOfGLType(md.type)) + md.offset;
    }
    glBindVertexArray(0);
}

void VAO::bind() {
    glBindVertexArray(m_handle);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &m_handle);
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
