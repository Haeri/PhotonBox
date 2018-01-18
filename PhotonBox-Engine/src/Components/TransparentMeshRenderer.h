#ifndef TRANSPARENT_MESH_RENDERER_H
#define TRANSPARENT_MESH_RENDERER_H

#include "MeshRenderer.h"

class TransparentMeshRenderer : public MeshRenderer
{
public:
	bool cutout = true;
	TransparentMeshRenderer() : MeshRenderer(false) {}
};

#endif // TRANSPARENT_MESH_RENDERER_H