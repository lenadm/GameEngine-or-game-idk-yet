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

void getFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool readFromFile(std::string pathToShader, std::string& outBuf);

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

int main() {
    if (!glfwInit()) { return 1; }
    GLFWwindow *window = glfwCreateWindow(1920, 1440, "GameEngine", 0, 0);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD!!!\n";
        return 1;
    }

    ENABLE_DEBUG_OUTPUT();

    glfwSetFramebufferSizeCallback(window, getFramebufferSizeCallback);
    glViewport(0, 0, 1920, 1080);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

    GLuint VAO = VaoBuilder()
        .bindVBO(VBO).bindEBO(EBO)
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

    GLuint texture0 = TextureBuilder()
        .fromFile(apr.resolvePath("container.jpg"), GL_TEXTURE_2D)
        .freeTexture()
        .build();
    GLuint texture1 = TextureBuilder()
        .fromFile(apr.resolvePath("awesomeface.png"), GL_TEXTURE_2D)
        .freeTexture()
        .build();

    glUseProgram(prog);
    glUniform1i(glGetUniformLocation(prog, "uTexture0"), 0);
    glUniform1i(glGetUniformLocation(prog, "uTexture1"), 1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(prog);
    glfwTerminate();
    return 0;
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
