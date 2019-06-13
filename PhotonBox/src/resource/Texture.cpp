#include "PhotonBox/resource/Texture.h"

#include <iostream>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

#include "PhotonBox/util/FileWatch.h"
#include "PhotonBox/core/TextureSerializer.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

Texture::Texture(std::string fileName, bool generateMipMaps, bool hdr)
{
	FileWatch::addToWatchList(fileName, this);

	std::cout << "Index Texture: " << fileName << std::endl;
	
	_fileName = fileName;
	_isMipMap = generateMipMaps;
	_isHDR = hdr;

	loadAsync();
}

Texture::~Texture()
{
	glDeleteTextures(1, &_texture);
}

void Texture::bind()
{
	bind(0);
}

void Texture::bind(unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture::loadFromFile()
{
	int numComponents;
	std::size_t found = _fileName.find_last_of(".");
	std::string cachePath = _fileName.substr(0, found) + TextureSerializer::EXTENSION;
	struct stat buffer;
	bool ispbt = false;

	if (stat(cachePath.c_str(), &buffer) == 0 && buffer.st_size > 0) {
		_data = TextureSerializer::read(cachePath, &_width, &_height, &numComponents);
		ispbt = true;
	}
	else
	{
		_data = stbi_load((_fileName).c_str(), &_width, &_height, &numComponents, STBI_rgb_alpha);
		TextureSerializer::write(_fileName.substr(0, found) + TextureSerializer::EXTENSION, _width, _height, 4, _data);
	}
}

void Texture::blankInitialize()
{
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	unsigned char r = 50;
	unsigned char g = 50;
	unsigned char b = 50;
	unsigned char a = 255;
	unsigned char dat[4] = { r, g, b, a };

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, dat);
}

void Texture::submitBuffer()
{
	GLint format = _isHDR ? GL_RGB16F : GL_RGBA;

	//glDeleteTextures(1, &_texture);
	//glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
	// TODO: Texture compression
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM, _width, _height, 0, GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM, GL_UNSIGNED_BYTE, data);
	//glCompressedTexImage2D(GL_COMPRESSED_RGBA_S3TC_DXT5_ANGLE)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (_isMipMap)
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

	TextureSerializer::free_buffer(_data);

	//std::cout << "Initialized: " << _fileName << "\n";
}

