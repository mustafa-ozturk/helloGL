#pragma once

#include <GL/glew.h>

class VertexBufferObject
{
public:
    VertexBufferObject() = delete;
    VertexBufferObject(const float* vertices)
    {
        glGenBuffers(1, &m_VertexBufferObjectID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjectID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }
private:
    unsigned int m_VertexBufferObjectID;
};