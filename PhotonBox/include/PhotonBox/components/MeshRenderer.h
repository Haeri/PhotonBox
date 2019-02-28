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

	void render() override;
	void render(Shader* shader) override;
	void render(Shader* shader, LightEmitter* light) override;

	BoundingSphere getBoundingSphere();
	Mesh* getMesh() { return _mesh; }

	MeshRenderer& setMesh(Mesh* mesh);
private:
	Mesh * _mesh;
};

#endif // MESH_RENDERER_H
