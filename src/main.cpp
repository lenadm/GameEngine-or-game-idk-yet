#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void getFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool getShaderSourceCode(std::filesystem::path pathToShader, std::string& buf);

const std::filesystem::path g_assetsRoot = std::filesystem::current_path() / "assets";

int main() {
    if (!glfwInit()) return 1;

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
    glViewport(0, 0, 1920, 1440);

    float verts[] = {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    std::string buf;
    if (!getShaderSourceCode(g_assetsRoot / "shaders/triangle.vert", buf)) {
        std::cout << buf;
    }
    const char* vertexShaderSource = buf.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
    }

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.39, 0.584, 0.929, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

bool getShaderSourceCode(std::filesystem::path pathToShader, std::string& buf) {
    if (!std::filesystem::exists(pathToShader)) {
        buf = "Shader does not exist at this path: " + pathToShader.string();
        return false;
    }

    if (!std::filesystem::is_regular_file(pathToShader)) {
        buf = "Shader path exists but is not a file: " + pathToShader.string();
        return false;
    }

    std::ifstream file(pathToShader);
    if (!file) {
        buf = "Unable to open Shader: " + pathToShader.string();
        return false;
    }

    buf = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return true;
}

void getFramebufferSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
