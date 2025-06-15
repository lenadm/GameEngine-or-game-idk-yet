#include <optional>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <filesystem>
#include <fstream>

#include "shader-program.h"
#include "shader-builder.h"

void getFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::optional<std::string> readFromFile(std::filesystem::path pathToShader);

const float verts[] = {
    //positions         colors
    0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // Top middle
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom left
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // Bottom Right
};

unsigned int triangleIndices[] = {
    0, 1, 2,
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

    glfwSetFramebufferSizeCallback(window, getFramebufferSizeCallback);
    glViewport(0, 0, 1920, 1080);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::optional<std::string> vertexSource = readFromFile("shaders/triangle.vert");
    std::optional<std::string> fragSource = readFromFile("shaders/triangle.frag");
    if ( !vertexSource || !fragSource ) {
        std::cout << "error Retrieving shader source code\n";
        return 1;
    }

    ShaderProgram prog = ShaderBuilder()
        .addShader(vertexSource.value(), GL_VERTEX_SHADER)
        .addShader(fragSource.value(), GL_FRAGMENT_SHADER)
        .buildProgram().value_or(ShaderProgram(0));
    if (!prog.id()) {
        std::cout << "error linking shader Program";
        return 1;
    }
    prog.bind();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        prog.uniformSetFloat("green", greenValue);

        glBindVertexArray(VAO);
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
    const std::filesystem::path assetsRoot = std::filesystem::current_path() / "assets";
    pathToShader = assetsRoot / pathToShader;

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
