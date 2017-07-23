#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "display.h"

class Texture {
public:
	Texture(const std::string& fileName);
	~Texture();
	void bind();
private:
	GLuint _texture;
};

#endif /* defined(TEXTURE_H) */
