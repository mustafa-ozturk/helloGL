#pragma once

#include <GL/glew.h>

class ElementBufferObject
{
public:
    ElementBufferObject() = delete;
    ElementBufferObject(const ElementBufferObject&) = delete;
    ElementBufferObject& operator=(const ElementBufferObject&) = delete;

    ElementBufferObject(const std::vector<unsigned int>& indices)
    {
        glGenBuffers(1, &m_ElementBufferObjectID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferObjectID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    }
    ~ElementBufferObject()
    {
        glDeleteBuffers(1, &m_ElementBufferObjectID);
    }
private:
    unsigned int m_ElementBufferObjectID = 0;
};