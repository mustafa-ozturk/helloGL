#pragma once

#include <GL/glew.h>

class VertexArrayObject
{
public:
    VertexArrayObject(const VertexArrayObject&) = delete;
    VertexArrayObject& operator=(const VertexArrayObject&) = delete;

    VertexArrayObject()
    {
        glGenVertexArrays(1, &m_VertexArrayObjectID);
        Bind();
    }
    ~VertexArrayObject()
    {
        glDeleteVertexArrays(1, &m_VertexArrayObjectID);
    }

    void Bind()
    {
        glBindVertexArray(m_VertexArrayObjectID);
    }
    void UnBind()
    {
        glBindVertexArray(0);
    }
private:
    unsigned int m_VertexArrayObjectID = 0;
};