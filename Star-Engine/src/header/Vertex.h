#ifndef VERTEX_H
#define VERTEX_H

#include "Vector3f.h"

class Vertex {
public:
	Vertex(Vector3f pos): 
		_pos(pos)
	{}

	Vector3f getPosition() { return _pos; }

private:
	Vector3f _pos;
};

#endif /* defined(VERTEX_H) */
