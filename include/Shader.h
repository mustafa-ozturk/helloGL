#pragma once

#include <GL/glew.h>
#include <iostream>

class Shader
{
public:
    Shader() = delete;
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(const char* vertexShader, const char* fragmentShader)
    {
        CreateVertexShader(vertexShader);
        CreateFragmentShader(fragmentShader);
        m_ShaderProgramID = glCreateProgram();
        glAttachShader(m_ShaderProgramID, m_VertexShaderID);
        glAttachShader(m_ShaderProgramID, m_FragmentShaderID);
        glLinkProgram(m_ShaderProgramID);
        // error checking
        int success;
        char infoLog[512];
        glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_ShaderProgramID, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(m_VertexShaderID);
        glDeleteShader(m_FragmentShaderID);
    }
    ~Shader()
    {
        glDeleteProgram(m_ShaderProgramID);
    }

    void UseShader() const
    {
        glUseProgram(m_ShaderProgramID);
    }

    unsigned int GetShaderProgram() const
    {
        return m_ShaderProgramID;
    }
private:
    void CreateVertexShader(const char* vertexShader)
    {
        m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_VertexShaderID, 1, &vertexShader, nullptr);
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
    void CreateFragmentShader(const char* fragmentShader)
    {
        m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_FragmentShaderID, 1, &fragmentShader, nullptr);
        glCompileShader(m_FragmentShaderID);
        // error checking
        int success;
        char infoLog[512];
        glGetShaderiv(m_FragmentShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(m_FragmentShaderID, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    unsigned int m_VertexShaderID = 0;
    unsigned int m_FragmentShaderID = 0;
    unsigned int m_ShaderProgramID = 0;
};