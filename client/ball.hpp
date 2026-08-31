#pragma once

#include <glm/glm.hpp>

struct Ball
{
    glm::vec2 pos;
    glm::vec2 size;
    glm::vec2 velocity;

    void update(float dt)
    {
        pos += velocity * dt;
    }
};