#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "shader.hpp"
#include "renderer.hpp"
#include "game.hpp"
#include "network_client.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float FIXED_DT = 1.0f / 60.0f;

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

// Entry point

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        return -1;
    }

    GLFWwindow* window = createWindow();
    if (!window) return -1;

    // Setup - Begin

    glm::mat4 projection = glm::ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);
    std::string vertPath = std::string(PROJECT_ROOT) + "/client/shaders/quad.vert";
    std::string fragPath = std::string(PROJECT_ROOT) + "/client/shaders/quad.frag";
    Shader quadShader(vertPath.c_str(), fragPath.c_str());
    QuadRenderer renderer(&quadShader, projection);

    PongGame game(WINDOW_WIDTH, WINDOW_HEIGHT);

    NetworkClient network("178.218.160.184", 7777);
    if (!network.join())
    {
        std::cerr << "Failed to join server\n";
        WSACleanup();
        return -1;
    }

    // Setup - End

    float accumulator = 0.0f;
    uint32_t currentTick = 0;

    while (!glfwWindowShouldClose(window))
    {
        float frameTime = getDeltaTime();
        accumulator += frameTime;

        while (accumulator >= FIXED_DT)
        {
            bool localUp, localDown;
            game.captureInput(window, localUp, localDown);

            network.sendInput(currentTick, localUp, localDown);
            currentTick++;

            std::optional<StatePacket> state = network.receiveState();
            if (state.has_value())
            {
                game.applyInput(state->player1Up, state->player1Down,
                                state->player2Up, state->player2Down, FIXED_DT);
                game.update(FIXED_DT);
            }

            accumulator -= FIXED_DT;
        }

        game.render(renderer);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    WSACleanup();
    return 0;
}