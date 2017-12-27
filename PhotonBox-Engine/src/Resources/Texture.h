#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "../src/Core/Display.h"

class Texture {
public:
	Texture(bool generateMipMaps);
	Texture(bool generateMipMaps, bool hdr);
	Texture(std::string & fileName);
	Texture(std::string& fileName, bool generateMipMaps);
	Texture(std::string& fileName, bool generateMipMaps, bool hdr);
	~Texture();
	void bind();
	void bind(GLuint textureUnit);
	int getWidth() { return _width; }
	int getHeight() { return _height; }
	bool isHDR() { return _isHDR; }
	std::string getName() { return _fileName; }
	GLuint getTextureLocation() { return _texture; }

	static unsigned char* loadIcon(const std::string& fileName, int& width, int& height);
private:
	bool _isHDR;
	GLuint _texture;
	int _width, _height;
	std::string _fileName;
	static unsigned int _nameIndex;

	void initializeTexture(unsigned char* data, bool generateMipMaps, bool hdr);
};

#endif // TEXTURE_H