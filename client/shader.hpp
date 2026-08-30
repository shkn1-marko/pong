#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    GLuint id;

    Shader(const char* vertPath, const char* fragPath)
    {
        std::string vertSrc = readFile(vertPath);
        std::string fragSrc = readFile(fragPath);

        GLuint vert = compile(GL_VERTEX_SHADER, vertSrc.c_str());
        GLuint frag = compile(GL_FRAGMENT_SHADER, fragSrc.c_str());

        id = glCreateProgram();
        glAttachShader(id, vert);
        glAttachShader(id, frag);
        glLinkProgram(id);

        int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(id, 512, nullptr, infoLog);
            std::cerr << "Shader link failed:\n" << infoLog << "\n";
        }

        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    void use() { glUseProgram(id); }

    void setMat4(const char* name, const float* value)
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, value);
    }

    void setVec3(const char* name, float x, float y, float z)
    {
        glUniform3f(glGetUniformLocation(id, name), x, y, z);
    }

private:
    std::string readFile(const char* path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "Failed to open shader file: " << path << "\n";
            return "";
        }
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    GLuint compile(GLenum type, const char* src)
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader compile failed ("
                      << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                      << "):\n"
                      << infoLog
                      << "\n";
        }
        return shader;
    }
};