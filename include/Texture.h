#pragma once

#include <GL/glew.h>
#include "stb_image/stb_image.h"

class Texture
{
public:
    Texture(std::string textureFilePath, GLenum target, GLenum format, std::string name)
        : m_TextureName(name), m_TextureTarget(target)
    {
        glGenTextures(1, &m_TextureID);
        Bind();
        stbi_set_flip_vertically_on_load(true);
        const char* c_str = textureFilePath.c_str();
        unsigned char* TextureData = stbi_load(
                c_str,
                &m_Width,
                &m_Height,
                &m_ColorChannels,
                0
                );
        if (TextureData)
        {
            glTexImage2D(
                    m_TextureTarget,
                    0,
                    GL_RGB,
                    m_Width,
                    m_Height,
                    0,
                    format,
                    GL_UNSIGNED_BYTE,
                    TextureData
                    );
            glGenerateMipmap(m_TextureTarget);
        }
        else
        {
            std::cout << "Failed to load texture: " << m_TextureID << std::endl;
        }
        stbi_image_free(TextureData);
    }
    void Bind()
    {
        glActiveTexture(GL_TEXTURE0 + m_TextureID - 1);
        glBindTexture(m_TextureTarget, m_TextureID);
    }
    void Set2DTextureWrapping(GLint sWrappingMode, GLint tWrappingMode)
    {
        glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_S, sWrappingMode);
        glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_T, tWrappingMode);
    }
    void Set2DTextureFiltering(GLint sFilteringMode, GLint tFilteringMode)
    {
        glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_S, sFilteringMode);
        glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_T, tFilteringMode);
    }
    std::string GetTextureName() const
    {
        return m_TextureName;
    }
    unsigned int GetTextureID() const
    {
        return m_TextureID;
    }
private:
    unsigned int m_TextureID = 0;
    int m_Width = 0;
    int m_Height = 0;
    int m_ColorChannels = 0;
    std::string m_TextureName;
    GLenum m_TextureTarget;
};