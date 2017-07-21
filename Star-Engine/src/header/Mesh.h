#ifndef MESCH_H
#define MESCH_H

#include <vector>
#include "Vertex.h"

class Mesh {
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
		this->vertices = vertices;
		this->indices = indices;
	}

	std::vector< Vertex > vertices;
	std::vector< unsigned int> indices;
};

#endif /* defined(MESCH_H) */
