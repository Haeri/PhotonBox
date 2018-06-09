#ifndef TRANSPARENT_MESH_RENDERER_H
#define TRANSPARENT_MESH_RENDERER_H

#include "PhotonBox/components/MeshRenderer.h"

class TransparentMeshRenderer : public MeshRenderer
{
public:
	TransparentMeshRenderer() : MeshRenderer(RenderType::transparent) {}
};

#endif // TRANSPARENT_MESH_RENDERER_H
