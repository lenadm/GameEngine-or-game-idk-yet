#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

#include "texture-builder.h"
#include "opengl-debug.h"

GLenum channelsToGLFormat(int channels);

TextureBuilder& TextureBuilder::freeTexture() {
    m_ownsData = true;
    return *this;
}

GLuint TextureBuilder::build() {
    GLuint newTex;
    glGenTextures(1, &newTex);
    glBindTexture(m_texType, newTex);
    glTexImage2D(m_texType, 0, GL_RGB, m_width, m_height, 0, m_colorChannels, GL_UNSIGNED_BYTE, m_texData);
    glGenerateMipmap(m_texType);

    if (m_ownsData) {
        stbi_image_free(m_texData);
    }
    return newTex;
}

TextureBuilder& TextureBuilder::fromFile(std::string path, GLenum texType) {
    stbi_set_flip_vertically_on_load(true);
    m_ownsData = true;
    m_texType = texType;
    int nrChannels; 
    m_texData = stbi_load(path.c_str(), &m_width, &m_height, &nrChannels, 0);
    m_colorChannels = channelsToGLFormat(nrChannels);
    if (!m_texData) {
        std::cout << "Failed to load texture\n";
    }
    return *this;
    stbi_set_flip_vertically_on_load(false);
}

GLenum channelsToGLFormat(int channels) {
    switch (channels) {
        case 1: return GL_RED;       // Grayscale
        case 2: return GL_RG;        // Grayscale + Alpha
        case 3: return GL_RGB;       // RGB
        case 4: return GL_RGBA;      // RGBA
    }
    RUNTIME_ASSERT(false);
    return 0;
}
