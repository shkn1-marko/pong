#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

class QuadRenderer
{
public:
    QuadRenderer(Shader* shader, const glm::mat4& projection)
        : shader(shader), projection(projection)
    {
        setupQuad();
    }

    void draw(glm::vec2 pos, glm::vec2 size, glm::vec3 color)
    {
        shader->use();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(pos, 0.0f));
        model = glm::scale(model, glm::vec3(size, 1.0f));

        shader->setMat4("uProjection", &projection[0][0]);
        shader->setMat4("uModel", &model[0][0]);
        shader->setVec3("uColor", color.r, color.g, color.b);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

private:
    Shader* shader;
    glm::mat4 projection;
    GLuint vao, vbo;

    void setupQuad()
    {
        float vertices[] =
        {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,

            0.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
        };

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }
};