#pragma once

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
    Shader() = delete;
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        std::string vertexShaderCode;
        std::string fragmentShaderCode;
        std::ifstream vertexShaderFile;
        std::ifstream fragmentShaderFile;
        // ensure ifstream objects can throw exceptions:
        vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vertexShaderFile.open(vertexShaderPath);
            fragmentShaderFile.open(fragmentShaderPath);
            std::stringstream vertexShaderStream, fragmentShaderStream;

            // read file's buffer contents into streams
            vertexShaderStream << vertexShaderFile.rdbuf();
            fragmentShaderStream << fragmentShaderFile.rdbuf();

            vertexShaderFile.close();
            fragmentShaderFile.close();

            vertexShaderCode = vertexShaderStream.str();
            fragmentShaderCode = fragmentShaderStream.str();
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SCCESFULLY_READ" << std::endl;
        }

        CompileVertexShader(vertexShaderCode);
        CompileFragmentShader(fragmentShaderCode);
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

    void setUniformBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_ShaderProgramID, name.c_str()), (int)value);
    }
    void setUniformInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
    }
    void setUniformFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
    }
private:
    void CompileVertexShader(const std::string& vertexShaderCode)
    {
        const char* c_str = vertexShaderCode.c_str();
        m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_VertexShaderID, 1, &c_str, nullptr);
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
    void CompileFragmentShader(const std::string& fragmentShaderCode)
    {
        const char* c_str = fragmentShaderCode.c_str();
        m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_FragmentShaderID, 1, &c_str, nullptr);
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