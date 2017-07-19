#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "ObjectRenderer.h"
#include "Mesh.h"
#include "Display.h"

class MeshRenderer : public ObjectRenderer {
public:
	void init() override;
	void render() override;
	void onDestroy() override;

	void setMesh(Mesh* mesh) { _mesh = mesh; }
private: 
	enum {
		POS_BUFFR,
		NUM_BUFFER
	};

	GLuint _vao;
	GLuint _vbo[NUM_BUFFER];
	Mesh* _mesh;
};

#endif /* defined(MESH_RENDERER_H) */