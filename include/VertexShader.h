#pragma once

#include <GL/glew.h>
#include <iostream>

class VertexShader
{
public:
    VertexShader() = delete;
    VertexShader(const char* vertexShaderSourceCode)
    {
        m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_VertexShaderID, 1, &vertexShaderSourceCode, nullptr);
        glCompileShader(m_VertexShaderID);
        // error checking
        int success;
        char infoLog[512];
        glGetShaderiv(m_VertexShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(m_VertexShaderID, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
private:
    unsigned int m_VertexShaderID;
};