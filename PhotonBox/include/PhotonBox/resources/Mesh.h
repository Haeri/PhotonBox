#ifndef MESH_H
#define MESH_H

#include <vector>

#include "PhotonBox/resources/Vertex.h"
#include "PhotonBox/core/BoundingSphere.h"
#include "PhotonBox/core/ManagedResource.h"
#include "PhotonBox/core/ILazyLoadable.h"
#include "PhotonBox/core/OpenGL.h"

class Mesh : public ManagedResource, public ILazyLoadable
{
public:
	std::vector< Vertex > vertices;
	std::vector< unsigned int> indices;
	BoundingSphere boundingSphere;

	Mesh(const std::string& fileName, bool forceInit = false);
	~Mesh();

	GLuint getVAO();
	GLuint getEBO();

	void sendToGPU() override;
private:
	GLuint _vao, _vbo, _ebo;
	std::string _fileName;

	void loadFromFile() override;
	void blankInitialize() override;
};

#endif // MESH_H
