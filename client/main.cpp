#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "shader.hpp"
#include "renderer.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Window / GL setup

GLFWwindow* createWindow()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW\n";
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return nullptr;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int xPos = (mode->width - WINDOW_WIDTH) / 2;
    int yPos = (mode->height - WINDOW_HEIGHT) / 2;
    glfwSetWindowPos(window, xPos, yPos);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to init GLAD\n";
        return nullptr;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    return window;
}

// Time

float getDeltaTime()
{
    static float lastFrame = 0.0f;
    float currentFrame = (float)glfwGetTime();
    float dt = currentFrame - lastFrame;
    lastFrame = currentFrame;
    return dt;
}

// Input

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Rendering

void render(QuadRenderer& renderer)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    renderer.draw(
        glm::vec2(50.0f, 250.0f),
        glm::vec2(20.0f, 100.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
}

// Entry point

int main()
{
    GLFWwindow* window = createWindow();
    if (!window) return -1;

    // Setup - Begin

    glm::mat4 projection = glm::ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);
    std::string vertPath = std::string(PROJECT_ROOT) + "/client/shaders/quad.vert";
    std::string fragPath = std::string(PROJECT_ROOT) + "/client/shaders/quad.frag";
    Shader quadShader(vertPath.c_str(), fragPath.c_str());
    QuadRenderer renderer(&quadShader, projection);

    // Setup - End

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        render(renderer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}