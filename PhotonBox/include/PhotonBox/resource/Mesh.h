#ifndef MESH_H
#define MESH_H

#include <vector>

#include "PhotonBox/core/OpenGL.h"
#include "PhotonBox/core/ManagedResource.h"
#include "PhotonBox/core/LazyLoadable.h"
#include "PhotonBox/data-type/Vertex.h"
#include "PhotonBox/data-type/BoundingSphere.h"

class Mesh : public ManagedResource, public LazyLoadable
{
public:
	std::vector< Vertex > vertices;
	std::vector< unsigned int> indices;
	BoundingSphere boundingSphere;

	Mesh(const std::string& filePath, bool forceInit = false);
	~Mesh();

	GLuint getVAO();
	GLuint getEBO();

	void submitBuffer() override;
private:
	GLuint _vao, _vbo, _ebo;

	void loadFromFile() override;
	void blankInitialize() override;
};

#endif // MESH_H
