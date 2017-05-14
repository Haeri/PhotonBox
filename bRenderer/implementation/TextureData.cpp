#include "headers/TextureData.h"

TextureData::TextureData(const std::string &fileName)
{
	load(fileName);
}


TextureData::TextureData(GLsizei width, GLsizei height, GLenum format, ImageDataPtr imageData)
{
	create(width, height, format, imageData);
}

TextureData &TextureData::create(GLsizei width, GLsizei height, GLenum format, ImageDataPtr imageData)
{
	_width = width;
	_height = height;
	_format = format;
	_imageData = imageData;

	return *this;
}