#include "game.hpp"

PongGame::PongGame(int windowWidth, int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight)
{
    leftPaddle = Paddle
    {
        glm::vec2(50.0f, 250.0f),
        glm::vec2(20.0f, 100.0f),
        300.0f
    };
    rightPaddle = Paddle
    {
        glm::vec2(windowWidth - 70.0f, 250.0f),
        glm::vec2(20.0f, 100.0f),
        300.0f
    };
    ball = Ball
    {
        glm::vec2(windowWidth / 2.0f - 10.0f, windowHeight / 2.0f - 10.0f),
        glm::vec2(20.0f, 20.0f),
        glm::vec2(200.0f, 150.0f)
    };
}

bool PongGame::checkCollision(const glm::vec2& posA, const glm::vec2& sizeA,
                              const glm::vec2& posB, const glm::vec2& sizeB)
{
    bool overlapX = posA.x < posB.x + sizeB.x && posA.x + sizeA.x > posB.x;
    bool overlapY = posA.y < posB.y + sizeB.y && posA.y + sizeA.y > posB.y;
    return overlapX && overlapY;
}

void PongGame::resetBall()
{
    ball.pos = glm::vec2(windowWidth / 2.0f - ball.size.x / 2.0f,
                         windowHeight / 2.0f - ball.size.y / 2.0f);
    ball.velocity = glm::vec2(200.0f, 150.0f);
}

void PongGame::processInput(GLFWwindow* window, float dt)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        leftPaddle.moveUp(dt, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        leftPaddle.moveDown(dt, (float)windowHeight);

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        rightPaddle.moveUp(dt, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        rightPaddle.moveDown(dt, (float)windowHeight);
}

void PongGame::update(float dt)
{
    if (gameOver) return;

    ball.update(dt, 0.0f, (float)windowHeight);

    if (checkCollision(ball.pos, ball.size, leftPaddle.pos, leftPaddle.size))
        ball.velocity.x = -ball.velocity.x;
    if (checkCollision(ball.pos, ball.size, rightPaddle.pos, rightPaddle.size))
        ball.velocity.x = -ball.velocity.x;

    if (ball.pos.x < 0.0f)
    {
        rightScore++;
        if (rightScore >= winningScore) gameOver = true;
        else resetBall();
    }
    if (ball.pos.x + ball.size.x > (float)windowHeight)
    {
        leftScore++;
        if (leftScore >= winningScore) gameOver = true;
        else resetBall();
    }
}

void PongGame::render(QuadRenderer& renderer)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawNet(renderer);

    renderer.draw(leftPaddle.pos, leftPaddle.size, glm::vec3(1.0f, 1.0f, 1.0f));
    renderer.draw(rightPaddle.pos, rightPaddle.size, glm::vec3(1.0f, 1.0f, 1.0f));
    renderer.draw(ball.pos, ball.size, glm::vec3(1.0f, 1.0f, 1.0f));

    drawScore(renderer);
}