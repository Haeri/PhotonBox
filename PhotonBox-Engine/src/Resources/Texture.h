#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "../src/Core/Display.h"

class Texture {
public:
	int width, height;

	Texture(const std::string& fileName, bool generateMipMaps);
	~Texture();
	void bind();
private:
	GLuint _texture;
};

#endif /* defined(TEXTURE_H) */
