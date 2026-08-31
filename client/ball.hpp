#pragma once

#include <glm/glm.hpp>

struct Ball
{
    glm::vec2 pos;
    glm::vec2 size;
    glm::vec2 velocity;

    void update(float dt, float minY, float maxY)
    {
        pos += velocity * dt;

        if (pos.y < minY)
        {
            pos.y = minY;
            velocity.y = -velocity.y;
        }
        if (pos.y + size.y > maxY)
        {
            pos.y = maxY - size.y;
            velocity.y = -velocity.y;
        }
    }
};