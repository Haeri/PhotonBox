#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include <string>
#include "../src/Core/Display.h"

class CubeMap {
public:
	CubeMap(const std::string & fileName);
	~CubeMap();
	void bind();
private:
	GLuint _texture;
	int _width, _height;
};

#endif // CUBE_MAP_H