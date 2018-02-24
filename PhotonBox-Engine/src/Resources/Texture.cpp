#include "Texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

unsigned int Texture::_nameIndex = 0;

Texture::Texture(bool generateMipMaps) : Texture(generateMipMaps, false) {}
Texture::Texture(bool generateMipMaps, bool hdr)
{
	_fileName = "Generated_Texture_" + std::to_string(_nameIndex++);
	initializeTexture(NULL, generateMipMaps, hdr);
}

Texture::Texture(std::string & fileName) : Texture(fileName, false, false) {}
Texture::Texture(std::string & fileName, bool generateMipMaps) : Texture(fileName, generateMipMaps, false) {}
Texture::Texture(std::string & fileName, bool generateMipMaps, bool hdr)
{
	int numComponents;
	unsigned char* data = stbi_load((fileName).c_str(), &_width, &_height, &numComponents, STBI_rgb_alpha);

	_fileName = fileName;

	if (data != NULL)
	{
		initializeTexture(data, generateMipMaps, hdr);
		stbi_image_free(data);
	}
	else
	{
		std::cerr << "Unable to load texture: " << fileName << std::endl;
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &_texture);
}

void Texture::bind()
{
	bind(GL_TEXTURE0);
}

void Texture::bind(GLuint textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, _texture);
}

unsigned char* Texture::loadIcon(const std::string& fileName, int& width, int& height)
{
	int numComponents;
	unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

	if (data == NULL)
		std::cerr << "Unable to load texture: " << fileName << std::endl;

	return data;
}

void Texture::initializeTexture(unsigned char * data, bool generateMipMaps, bool hdr)
{
	_isHDR = hdr;
	GLint format = hdr ? GL_RGB16F : GL_RGBA;

	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	// TODO: Texture compression
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM, _width, _height, 0, GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (generateMipMaps)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}
