#include "Texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

Texture::Texture(const std::string & fileName) : Texture(fileName, false) {}

Texture::Texture(const std::string & fileName, bool generateMipMaps)
{
	int numComponents;
	unsigned char* data = stbi_load((fileName).c_str(), &_width, &_height, &numComponents, STBI_rgb_alpha);
	_fileName = fileName;

	if (data == NULL)
		std::cerr << "Unable to load texture: " << fileName << std::endl;

	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if(generateMipMaps){
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}else {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	stbi_image_free(data);
}

Texture::~Texture(){
	glDeleteTextures(1, &_texture);
}

void Texture::bind(){
	bind(GL_TEXTURE0);
}

void Texture::bind(GLuint textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, _texture);

	/*
	std::cout << _fileName << std::endl;
	std::cout << "glActiveTexture(" << "GL_TEXTURE" << textureUnit - GL_TEXTURE0 << ");" << std::endl << "glBindTexture(GL_TEXTURE_2D, " << _texture << ");" << std::endl;
	*/
}

unsigned char* Texture::loadIcon(const std::string& fileName, int& width, int& height) {
	int numComponents;
	unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

	if (data == NULL)
		std::cerr << "Unable to load texture: " << fileName << std::endl;

	return data;
}