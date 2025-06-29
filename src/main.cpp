#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <math.h>
#include <filesystem>

#include "shader-builder.h"
#include "vao-builder.h"
#include "opengl-debug.h"
#include "asset-path-resolver.h"
#include "texture-builder.h"

struct RenderingContext {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};

const float verts[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

const unsigned int triangleIndices[] = {
    0, 1, 2,
    2, 3, 0,
};

void initGLFW();
GLFWwindow* initWindow(int width, int height, const char* name, GLFWframebuffersizefun frameBufferCallback);
void initGlad();
GLuint createGLBuffer(GLenum bufType, size_t dataSize, const void* data, GLenum drawType);
void useTexture(GLenum textureNum, GLuint texture, GLenum textureType);
void useRenderingContext(GLuint VAO, GLuint VBO, GLuint EBO);
void useRenderingContext(RenderingContext ctx);
void getFramebufferSizeCallback(GLFWwindow*, int width, int height);
void processInput(GLFWwindow* window);
bool readFromFile(std::string pathToShader, std::string& outBuf);

int main() {
    // Init
    initGLFW();
    GLFWwindow* window = initWindow(1920, 1440, "GameEngine", getFramebufferSizeCallback);
    initGlad();
    ENABLE_DEBUG_OUTPUT();
    
    // Setup
    RenderingContext singleCtx;
    singleCtx.VBO = createGLBuffer(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    singleCtx.EBO = createGLBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);
    singleCtx.VAO = VaoBuilder()
        .bindVBO(singleCtx.VBO).bindEBO(singleCtx.EBO)
        .addBufferAttributes(0, 3, GL_FLOAT)
        .addBufferAttributes(1, 3, GL_FLOAT)
        .addBufferAttributes(2, 2, GL_FLOAT)
        .build();

    AssetPathResolver apr;
    apr.populateRegistry();

    GLuint prog = ShaderBuilder()
        .addShaderFromPath(apr.resolvePath("triangle.vert"), GL_VERTEX_SHADER)
        .addShaderFromPath(apr.resolvePath("triangle.frag"), GL_FRAGMENT_SHADER)
        .build();
    if (!prog) {
        std::cout << "error linking shader Program";
        return 1;
    }

    GLuint containerTex = TextureBuilder()
        .fromFile(apr.resolvePath("container.jpg"), GL_TEXTURE_2D)
        .freeTexture()
        .build();
    GLuint happyFaceTex = TextureBuilder()
        .fromFile(apr.resolvePath("awesomeface.png"), GL_TEXTURE_2D)
        .freeTexture()
        .build();

    glUseProgram(prog);
    glUniform1i(glGetUniformLocation(prog, "uTexture0"), 0);
    glUniform1i(glGetUniformLocation(prog, "uTexture1"), 1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        useTexture(GL_TEXTURE0, containerTex, GL_TEXTURE_2D);
        useTexture(GL_TEXTURE1, happyFaceTex, GL_TEXTURE_2D);

        useRenderingContext(singleCtx);
        int EBOLength = sizeof(triangleIndices) / sizeof(triangleIndices[0]);
        glDrawElements(GL_TRIANGLES, EBOLength, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void initGLFW() {
    if (!glfwInit()) {
        std::cout << "failed to init glfw\n";
        std::abort();
    }
}

GLFWwindow* initWindow(int width, int height, const char* name, GLFWframebuffersizefun frameBufferCallback) {
    GLFWwindow *window = glfwCreateWindow(width, height, name, 0, 0);
    if (!window) {
        std::cout << "Unable to create window\n";
        std::abort();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferCallback);
    return window;
}

void initGlad() {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD!!!\n";
        std::abort();
    }
}

GLuint createGLBuffer(GLenum bufType, size_t dataSize, const void* data, GLenum drawType) {
    GLuint newBuffer;
    glGenBuffers(1, &newBuffer);
    glBindBuffer(bufType, newBuffer);
    glBufferData(bufType, dataSize, data, drawType);
    return newBuffer;
}

void useTexture(GLenum textureNum, GLuint texture, GLenum textureType) {
        glActiveTexture(textureNum);
        glBindTexture(textureType, texture);
}

void useRenderingContext(GLuint VAO, GLuint VBO, GLuint EBO) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void useRenderingContext(RenderingContext ctx) {
    useRenderingContext(ctx.VAO, ctx.VBO, ctx.EBO);
}

void getFramebufferSizeCallback(GLFWwindow*, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

bool readFromFile(std::string pathToShader, std::string& outBuf) {
    if (!std::filesystem::exists(pathToShader)) {
        std::cout << "Shader does not exist at this path: " + pathToShader << "\n";
        return false;
    } 

    if (!std::filesystem::is_regular_file(pathToShader)) {
        std::cout << "Shader path exists but is not a file: " + pathToShader << "\n";
        return false;
    }

    std::ifstream file(pathToShader);
    if (!file) {
        std::cout << "Unable to open shader: " + pathToShader << "\n";
        return false;
    }

    outBuf = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return true; 
}

