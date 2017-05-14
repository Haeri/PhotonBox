#include "headers/Texture.h"
#include "headers/TextureData.h"

Texture::Texture(const TextureData &data)
{
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	if (_textureID)
	{
        if (data.getImageData()){
			// Create texture and mipmap
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, data.getFormat(), data.getWidth(), data.getHeight(), 0, data.getFormat(), GL_UNSIGNED_BYTE, data.getImageData().get());

            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else{
            // Create empty texture (e.g. for binding to a framebuffer) without mipmap
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, data.getFormat(), data.getWidth(), data.getHeight(), 0, data.getFormat(), GL_UNSIGNED_BYTE, NULL);
        }
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(GLint texUnit)
{
    glActiveTexture(texUnit);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureID);
}