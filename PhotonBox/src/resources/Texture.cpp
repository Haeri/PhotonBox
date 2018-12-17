#include "PhotonBox/resources/Texture.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

#include "PhotonBox/core/TextureSerializer.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

unsigned int Texture::_nameIndex = 0;

// TODO: Why is this here? There is no way to set width and height after the fact. THere is nit even a way to call initializeTexture
Texture::Texture(bool generateMipMaps, bool hdr)
{
	_fileName = "Generated_Texture_" + std::to_string(_nameIndex++);
	initializeTexture(NULL, generateMipMaps, hdr);
}

Texture::Texture(std::string fileName, bool generateMipMaps, bool hdr)
{
	std::cerr << "Loading Texture: " << fileName << std::endl;

	int numComponents;
	unsigned char* data;


	std::size_t found = fileName.find_last_of(".");
	std::string cachePath = fileName.substr(0, found) + TextureSerializer::EXTENSION;
	struct stat buffer;
	bool ispbt = false;

	if (stat(cachePath.c_str(), &buffer) == 0) {
		data = TextureSerializer::read(cachePath, &_width, &_height, &numComponents);
		ispbt = true;
	}
	else
	{
		data = stbi_load((fileName).c_str(), &_width, &_height, &numComponents, STBI_rgb_alpha);
		TextureSerializer::write(fileName.substr(0, found) + TextureSerializer::EXTENSION, _width, _height, 4, data);
	}

	_fileName = fileName;

	if (data != NULL)
	{
		initializeTexture(data, generateMipMaps, hdr);
		if (ispbt)
		{
			TextureSerializer::free_buffer(data);
		}
		else
		{
			stbi_image_free(data);
		}
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
	bind(0);
}

void Texture::bind(GLuint textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, _texture);
}

unsigned char* Texture::loadIcon(const std::string& fileName, int& width, int& height)
{
	int numComponents;
	unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

	if (data == NULL)
		std::cerr << "Unable to load Icon: " << fileName << std::endl;

	return data;
}

void Texture::freeIcon(unsigned char* data) 
{
	if (data == NULL) 
	{
		std::cerr << "Unable to free icon" << std::endl;
		return;
	}

	stbi_image_free(data);
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
	//glCompressedTexImage2D(GL_COMPRESSED_RGBA_S3TC_DXT5_ANGLE)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (generateMipMaps)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
}
