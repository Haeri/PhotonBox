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
	GLuint getLocation() { return _cubeMaps.at(0); }
	GLuint getLocation(int lod) { return _cubeMaps.at(lod); }
	void bind();
	void bind(GLenum textureUnit);
	void bind(GLenum textureUnit, int lod);
	int getMaxLod() { return _maxLod; }
private:
	int _maxLod = 0;
	std::vector<GLuint> _cubeMaps;

};

#endif // CUBE_MAP_H