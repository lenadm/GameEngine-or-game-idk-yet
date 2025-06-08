#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

void getFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const float verts[] = {
    0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};

unsigned int triangleIndices[] = {
    0, 1, 2,
    1, 2, 3,
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

    Shader::handle vertexShader = Shader::make("shaders/triangle.vert", GL_VERTEX_SHADER);
    Shader::handle fragShader = Shader::make("shaders/triangle.frag", GL_FRAGMENT_SHADER);
    unsigned int shaderProgram = 0;
    {
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragShader);
        glLinkProgram(shaderProgram);
    } 
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);


    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.39, 0.584, 0.929, 1);
        glUseProgram(shaderProgram);
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

void getFramebufferSizeCallback(GLFWwindow* window, int width, int height){
    (void) window;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
