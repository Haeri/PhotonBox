#ifndef MESH_H
#define MESH_H

#include <vector>
#include "PhotonBox/resources/Vertex.h"
#include "PhotonBox/core/BoundingSphere.h"

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

#endif // MESH_H
