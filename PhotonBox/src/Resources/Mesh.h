#ifndef MESCH_H
#define MESCH_H

#include <vector>
#include <limits>
#include "Vertex.h"

class Mesh
{
public:
	std::vector< Vertex > vertices;
	std::vector< unsigned int> indices;

	float boundingSphereRadius;
	Vector3f min = Vector3f((std::numeric_limits<float>::max)());
	Vector3f max = Vector3f((std::numeric_limits<float>::min)());

	Mesh() {}
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
	}
};

#endif /* defined(MESCH_H) */
