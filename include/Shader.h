#pragma once

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>

class ShaderProgramSource
{
public:
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:
    Shader() = delete;

    Shader(const Shader&) = delete;

    Shader& operator=(const Shader&) = delete;

    Shader(const std::string& filepath)
    {
        ShaderProgramSource source = ParseShader(filepath);
        m_ShaderProgramID = CreateShader(source.VertexSource, source.FragmentSource);
    }

    ~Shader()
    {
        glDeleteProgram(m_ShaderProgramID);
    }

    void UseShader() const
    {
        glUseProgram(m_ShaderProgramID);
    }

    void Bind() const
    {
        glUseProgram(m_ShaderProgramID);
    }

    void UnBind() const
    {
        glUseProgram(0);
    }

    void setUniformBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_ShaderProgramID, name.c_str()), (int) value);
    }

    void setUniformInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
    }

    void setUniformFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
    }
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
    }

private:
    ShaderProgramSource ParseShader(const std::string& filepath)
    {
        std::ifstream stream(filepath);

        enum class ShaderType
        {
            NONE = -1,
            VERTEX = 0,
            FRAGMENT = 1
        };

        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;
        while (getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                {
                    type = ShaderType::VERTEX;
                } else if (line.find("fragment") != std::string::npos)
                {
                    type = ShaderType::FRAGMENT;
                }
            } else
            {
                ss[(int) type] << line << '\n';
            }
        }
        return {
                ss[0].str(),
                ss[1].str()
        };
    }

    unsigned int CompileShader(unsigned int type, const std::string& source)
    {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        // error handling
        // --------------------------------------------------------------
        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            // alloca lets you allocate on the stack dynamically
            char* message = (char*) alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile"
                      << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
                      << "shader !" << std::endl;
            std::cout << message << std::endl;
            glDeleteShader(id);
            return 0;
        }
        return id;
    }

    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        unsigned int program = glCreateProgram();

        // compile the shaders
        // ----------------------------------------------------------------------------
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        // link the shaders
        // -------------------------
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        // delete the intermediates (kinda like object files)
        // --------------------------------------------------
        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

private:
    unsigned int m_ShaderProgramID = 0;
};