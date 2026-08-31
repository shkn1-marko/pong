#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "shader.hpp"
#include "renderer.hpp"
#include "paddle.hpp"
#include "ball.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int WINNING_SCORE = 3;

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

// Collision

bool checkCollision(const glm::vec2& posA, const glm::vec2& sizeA,
                    const glm::vec2& posB, const glm::vec2& sizeB)
{
    bool overlapX = posA.x < posB.x + sizeB.x && posA.x + sizeA.x > posB.x;
    bool overlapY = posA.y < posB.y + sizeB.y && posA.y + sizeA.y > posB.y;
    return overlapX && overlapY;
}

// Reset

void resetBall(Ball& ball)
{
    ball.pos = glm::vec2(WINDOW_WIDTH / 2.0F - ball.size.x / 2.0f, WINDOW_HEIGHT / 2.0f - ball.size.y / 2.0f);
    ball.velocity = glm::vec2(200.0f, 150.0f);
}

// Input

void processInput(GLFWwindow* window, Paddle& left, Paddle& right, float dt)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        left.moveUp(dt, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        left.moveDown(dt, (float)WINDOW_HEIGHT);

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        right.moveUp(dt, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        right.moveDown(dt, (float)WINDOW_HEIGHT);
}

// Update

void updateGame(Ball& ball, Paddle& left, Paddle& right, float dt, int& leftScore, int& rightScore, bool& gameOver)
{
    if (gameOver) return;

    ball.update(dt, 0.0f, (float)WINDOW_HEIGHT);

    if (checkCollision(ball.pos, ball.size, left.pos, left.size))
    {
        ball.velocity.x = -ball.velocity.x;
    }
    if (checkCollision(ball.pos, ball.size, right.pos, right.size))
    {
        ball.velocity.x = -ball.velocity.x;
    }

    if (ball.pos.x < 0.0f)
    {
        rightScore++;
        if (rightScore >= WINNING_SCORE) gameOver = true;
        else resetBall(ball);
    }
    if (ball.pos.x + ball.size.x > (float)WINDOW_WIDTH)
    {
        leftScore++;
        if (leftScore >= WINNING_SCORE) gameOver = true;
        else resetBall(ball);
    }
}

// Rendering

void render(QuadRenderer& renderer, Paddle& left, Paddle& right, Ball& ball)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    renderer.draw(left.pos, left.size, glm::vec3(1.0f, 1.0f, 1.0f));
    renderer.draw(right.pos, right.size, glm::vec3(1.0f, 1.0f, 1.0f));
    renderer.draw(ball.pos, ball.size, glm::vec3(1.0f, 1.0f, 1.0f));
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

    Paddle leftPaddle
    {
        glm::vec2(50.0f, 250.0f),
        glm::vec2(20.0f, 100.0f),
        300.0f
    };
    Paddle rightPaddle
    {
        glm::vec2(WINDOW_WIDTH - 70.0f, 250.0f),
        glm::vec2(20.0f, 100.0f),
        300.0f
    };
    Ball ball
    {
        glm::vec2(WINDOW_WIDTH / 2.0f - 10.0f, WINDOW_HEIGHT / 2.0f - 10.0f),
        glm::vec2(20.0f, 20.0f),
        glm::vec2(200.0f, 150.0f)
    };

    int leftScore = 0;
    int rightScore = 0;

    bool gameOver = false;

    // Setup - End

    while (!glfwWindowShouldClose(window))
    {
        float dt = getDeltaTime();
        processInput(window, leftPaddle, rightPaddle, dt);
        updateGame(ball, leftPaddle, rightPaddle, dt, leftScore, rightScore, gameOver);
        render(renderer, leftPaddle, rightPaddle, ball);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}