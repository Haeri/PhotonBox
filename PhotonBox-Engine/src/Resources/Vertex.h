#ifndef VERTEX_H
#define VERTEX_H

#include "../Math/Vector3f.h"
#include "../Math/Vector2f.h"

class Vertex {
public:
	Vector3f position;
	Vector3f normal;
	Vector3f color;
	Vector2f uv;

	Vertex() {}

	Vertex(Vector3f pos, Vector3f normal, Vector3f col, Vector2f uv) :
		position(pos), 
		normal(normal), 
		color(col),
		uv(uv)
	{}
};

#endif /* defined(VERTEX_H) */
