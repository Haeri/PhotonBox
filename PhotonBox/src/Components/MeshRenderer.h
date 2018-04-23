#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

class Texture;
#include "../Resources/Material.h"
#include "../Resources/Mesh.h"
#include "ObjectRenderer.h"

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

	MeshRenderer& setMesh(Mesh* mesh);
	Mesh* getMesh() { return _mesh; }
private:
	Mesh * _mesh;

	//AABB* _lightProbeVolume;
	GLuint _vao, _vbo, _ebo;
};

#endif // MESH_RENDERER_H
