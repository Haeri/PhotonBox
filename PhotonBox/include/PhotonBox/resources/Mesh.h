#ifndef MESH_H
#define MESH_H

#include <vector>

#include "PhotonBox/resources/Vertex.h"
#include "PhotonBox/core/BoundingSphere.h"
#include "PhotonBox/resources/OBJLoader.h"
#include "PhotonBox/core/ManagedResource.h"
#include "PhotonBox/core/ILazyLoadable.h"

class Mesh : public ManagedResource, public ILazyLoadable
{
public:
	std::vector< Vertex > vertices;
	std::vector< unsigned int> indices;
	BoundingSphere boundingSphere;

	Mesh(const std::string& fileName) {
		OBJLoader::loadObj(fileName, this);
	}
	~Mesh() {}

	void loadFromFile() override { /* NOT IMPLEMENTED */ __debugbreak(); }
	void sendToGPU() override { /* NOT IMPLEMENTED */ __debugbreak(); }
};

#endif // MESH_H
