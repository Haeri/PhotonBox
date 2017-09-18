#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "ObjectRenderer.h"
#include "../Resources/Mesh.h"
#include "../Resources/Material.h"

class MeshRenderer : public ObjectRenderer {
public:
	void init() override;
	void render() override;
	void onDestroy() override;

	MeshRenderer& setMesh(Mesh* mesh) { _mesh = mesh; return *this; }
	MeshRenderer& setMaterial(Material* material) { _material = material; return *this; }
private: 
	Mesh* _mesh;
	Material* _material;

	GLuint _vao, _vbo, _ebo;
	
	Texture* default_normal, *default_specular;
};

#endif /* defined(MESH_RENDERER_H) */
