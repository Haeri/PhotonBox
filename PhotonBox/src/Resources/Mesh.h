#ifndef MESCH_H
#define MESCH_H

#include <vector>
#include "Vertex.h"
#include "../Core/BoundingSphere.h"

class Mesh
{
public:
	std::vector< Vertex > vertices;
	std::vector< unsigned int> indices;
	BoundingSphere boundingSphere;

	Mesh() {}
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
	}
};

#endif /* defined(MESCH_H) */
