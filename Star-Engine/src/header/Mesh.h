#ifndef MESCH_H
#define MESCH_H

#include "Vertex.h"

class Mesh {
public:
	void init(Vertex* vertices, unsigned int length) {
		_vertices = vertices;
		_length = length;
	}

	Vertex* getVertices() { return _vertices; }
	unsigned int getVertexLength() { return _length; }
private:
	unsigned int _length;
	Vertex* _vertices;
};

#endif /* defined(MESCH_H) */
