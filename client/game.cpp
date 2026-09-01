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