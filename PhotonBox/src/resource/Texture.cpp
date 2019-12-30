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

Texture::Texture(Config config)
	: _config(config)
{
	blankInitialize();
}

Texture::Texture(Filepath filePath, Config config)
	: _config(config)
{
	FileWatch::addToWatchList(filePath.getAbsolutePath(), this);
	_filePath = filePath;

	std::cout << "Index Texture: " << filePath.getAbsolutePath() << std::endl;

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

int Texture::getWidth()
{
	return _config.width;
}

int Texture::getHeight()
{
	return _config.height;
}

bool Texture::isHDR()
{
	return _config.hdr;
}

void Texture::setData(unsigned char* data)
{
	_data = data;
}

bool Texture::loadFromFile()
{
	int numComponents;

	std::string cachePath = _filePath.getPath() + _filePath.getName() + TextureSerializer::EXTENSION;
	struct stat buffer;

	if (stat(cachePath.c_str(), &buffer) == 0 && buffer.st_size > 0) 
	{
		_data = TextureSerializer::read(cachePath, &_config.width, &_config.height, &numComponents);
	}
	else
	{
		_data = stbi_load((_filePath.getAbsolutePath()).c_str(), &_config.width, &_config.height, &numComponents, STBI_rgb_alpha);
		TextureSerializer::write(cachePath, _config.width, _config.height, 4, _data);
	}

	return _data != nullptr;
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
	GLint format = _config.hdr ? GL_RGB16F : GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexImage2D(GL_TEXTURE_2D, 0, format, _config.width, _config.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
	// TODO: Texture compression
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM, _width, _height, 0, GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM, GL_UNSIGNED_BYTE, data);
	//glCompressedTexImage2D(GL_COMPRESSED_RGBA_S3TC_DXT5_ANGLE)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (_config.mips)
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

