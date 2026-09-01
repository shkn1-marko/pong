#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "paddle.hpp"
#include "ball.hpp"
#include "renderer.hpp"

class PongGame
{
public:
    PongGame(int windowWidth, int windowHeight);

    void processInput(GLFWwindow* window, float dt);
    void update(float dt);
    void render(QuadRenderer& renderer);

    bool isGameOver() const { return gameOver; }

private:
    int windowWidth;
    int windowHeight;
    int winningScore = 3;

    Paddle leftPaddle;
    Paddle rightPaddle;
    Ball ball;

    int leftScore = 0;
    int rightScore = 0;
    bool gameOver = false;

    static bool DIGIT_SEGMENTS[4][7];

    bool checkCollision(const glm::vec2& posA, const glm::vec2& sizeA,
                        const glm::vec2& posB, const glm::vec2& sizeB);

    void resetBall();
    void drawDigit(QuadRenderer& renderer, int digit, glm::vec2 pos, float scale, glm::vec3 color);
    void drawScore(QuadRenderer& renderer);
    void drawNet(QuadRenderer& renderer);
};