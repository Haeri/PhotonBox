#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

class Texture;

#include "PhotonBox/resources/Material.h"
#include "PhotonBox/resources/Mesh.h"
#include "PhotonBox/components/ObjectRenderer.h"

class MeshRenderer : public ObjectRenderer
{
public:
	MeshRenderer(RenderType type) :ObjectRenderer(type) {}
	MeshRenderer() : ObjectRenderer(RenderType::opaque) {}

	void init() override;
	void render() override;
	void render(Shader* shader) override;
	void render(Shader* shader, LightEmitter* light) override;
	void onDestroy() override;

	BoundingSphere getBoundingSphere();
	Mesh* getMesh() { return _mesh; }

	MeshRenderer& setMesh(Mesh* mesh);
private:
	Mesh * _mesh;
	GLuint _vao, _vbo, _ebo;
};

#endif // MESH_RENDERER_H
