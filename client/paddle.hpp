#pragma once

#include <glm/glm.hpp>

struct Paddle
{
    glm::vec2 pos;
    glm::vec2 size;
    float speed;

    void moveUp(float dt, float minY)
    {
        pos.y -= speed * dt;
        if (pos.y < minY) pos.y = minY;
    }

    void moveDown(float dt, float maxY)
    {
        pos.y += speed * dt;
        if (pos.y + size.y > maxY) pos.y = maxY - size.y;
    }
};