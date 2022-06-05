#pragma once

#include <GL/glew.h>
#include <iostream>

class VertexBufferObject
{
public:
    VertexBufferObject() = delete;
    VertexBufferObject(const VertexBufferObject&) = delete;
    VertexBufferObject& operator=(const VertexBufferObject&) = delete;

    VertexBufferObject(const float* vertices, const unsigned int size)
    {
        glGenBuffers(1, &m_VertexBufferObjectID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjectID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }
    ~VertexBufferObject()
    {
        glDeleteBuffers(1, &m_VertexBufferObjectID);
    }

    void UnBind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
private:
    unsigned int m_VertexBufferObjectID = 0;
};