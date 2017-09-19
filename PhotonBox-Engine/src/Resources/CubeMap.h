#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include <string>
#include <vector>
#include "../src/Core/Display.h"

class CubeMap {
public:
	enum Face
	{
		RIGHT,
		LEFT,
		TOP,
		BOTTOM,
		BACK,
		FRONT
	};

	CubeMap(const std::vector<std::string>& faces);
	~CubeMap();
	GLuint getLocation() { return _cubeMap; }
	void bind();
private:
	GLuint _cubeMap;
};

#endif // CUBE_MAP_H