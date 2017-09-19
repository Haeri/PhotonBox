#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "../src/Core/Display.h"

class Texture {
public:
	Texture(const std::string & fileName);
	Texture(const std::string& fileName, bool generateMipMaps);
	~Texture();
	void bind();
	void bind(GLuint textureUnit);
	int getWidth() { return _width; }
	int getHeight() { return _height; }

	static unsigned char* loadIcon(const std::string& fileName, int& width, int& height);
private:
	GLuint _texture;
	int _width, _height;
};

#endif // TEXTURE_H