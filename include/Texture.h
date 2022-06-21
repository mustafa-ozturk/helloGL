#pragma once

#include <GL/glew.h>
#include "stb_image/stb_image.h"

class Texture
{
public:
    Texture() = delete;
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(const std::string& textureFilePath)
    {
        stbi_set_flip_vertically_on_load(true);

        // rgba == desired channels 4
        unsigned char* TextureData = stbi_load(
                textureFilePath.c_str(),
                &m_Width,
                &m_Height,
                &m_ColorChannels,
                0
                );

        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // minification filter ( how it will be resampled down )
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // magnification filter ( how it will be scaled up )
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // horizontal wrap (dont extend the area )
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // vertical wrap (dont extend the area )

        // GL_RGBA8 (8 bits per channel)
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGB8,
                m_Width,
                m_Height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                TextureData
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(TextureData);
    }
    ~Texture()
    {
        glDeleteTextures(1, &m_TextureID);
    }

    void Bind(unsigned int slot = 0)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }
    void UnBind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
private:
    unsigned int m_TextureID = 0;
    int m_Width = 0;
    int m_Height = 0;
    int m_ColorChannels = 0;
};