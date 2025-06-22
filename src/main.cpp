#include <optional>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <filesystem>
#include <fstream>
#include <stb_image.h>

#include "shader-program.h"
#include "shader-builder.h"
#include "vao.h"
#include "opengl-debug.h"

void getFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::optional<std::string> readFromFile(std::filesystem::path pathToShader);

const std::filesystem::path g_assetsRoot = std::filesystem::current_path() / "assets";

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

    VAO vao;
    vao.bindVBO(VBO);
    vao.bindEBO(EBO);
    vao.addBufferAttributes(0, 3, GL_FLOAT);
    vao.addBufferAttributes(1, 3, GL_FLOAT);
    vao.addBufferAttributes(2, 2, GL_FLOAT);
    vao.setBufferAttributes();

    std::optional<std::string> vertexSource = readFromFile("shaders/triangle.vert");
    std::optional<std::string> fragSource = readFromFile("shaders/triangle.frag");
    if ( !vertexSource || !fragSource ) {
        std::cout << "error Retrieving shader source code\n";
        return 1;
    }

    std::optional<ShaderProgram> maybeProg = ShaderBuilder()
        .addShader(vertexSource.value(), GL_VERTEX_SHADER)
        .addShader(fragSource.value(), GL_FRAGMENT_SHADER)
        .buildProgram();
    if (!maybeProg) {
        std::cout << "error linking shader Program";
        return 1;
    }
    ShaderProgram prog = std::move(*maybeProg);
    prog.bind();

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(
        (g_assetsRoot / (std::filesystem::path) "textures/container.jpg").string().c_str(),
        &width, &height, &nrChannels, 0
    );
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture\n";
    }
    stbi_image_free(data);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);
        vao.bind();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
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

std::optional<std::string> readFromFile(std::filesystem::path pathToShader) {
    pathToShader = g_assetsRoot / pathToShader;

    if (!std::filesystem::exists(pathToShader)) {
        std::cout << "Shader does not exist at this path: " + pathToShader.string() << "\n";
        return std::nullopt;
    } 

    if (!std::filesystem::is_regular_file(pathToShader)) {
        std::cout << "Shader path exists but is not a file: " + pathToShader.string() << "\n";
        return std::nullopt;
    }

    std::ifstream file(pathToShader);
    if (!file) {
        std::cout << "Unable to open shader: " + pathToShader.string() << "\n";
        return std::nullopt;
    }

    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}
