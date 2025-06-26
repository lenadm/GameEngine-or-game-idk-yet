#pragma once

#include <GLFW/glfw3.h>
#include <string>

class TextureBuilder {
public:
    TextureBuilder() = default;
    TextureBuilder(GLenum texType, size_t width, size_t height, GLenum colorChannels, unsigned char* data):
    m_texType(texType), m_width(width), m_height(height), m_colorChannels(colorChannels), m_texData(data) {}

    TextureBuilder& fromFile(const std::string pathToFile, GLenum texType);
    TextureBuilder& freeTexture();

    GLuint build();
private:
    GLenum m_texType;
    int m_width;
    int m_height;
    GLenum m_colorChannels;
    unsigned char* m_texData;
    bool m_ownsData = false;
};
