#pragma once

#include <GL/glew.h>

class ElementBufferObject
{
public:
    ElementBufferObject(const ElementBufferObject&) = delete;
    ElementBufferObject& operator=(const ElementBufferObject&) = delete;

    ElementBufferObject(const unsigned int* indices, const unsigned int size)
    {
        glGenBuffers(1, &m_ElementBufferObjectID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferObjectID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    }
    ~ElementBufferObject()
    {
        glDeleteBuffers(1, &m_ElementBufferObjectID);
    }
private:
    unsigned int m_ElementBufferObjectID = 0;
};