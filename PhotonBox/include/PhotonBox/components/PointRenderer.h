#ifndef POINT_RENDERER_H
#define POINT_RENDERER_H

#include "PhotonBox/core/Entity.h"
#include "PhotonBox/core/OpenGL.h"
#include "PhotonBox/components/Camera.h"
#include "PhotonBox/components/ObjectRenderer.h"
#include "PhotonBox/components/Transform.h"
#include "PhotonBox/resources/CircleShader.h"

class PointRenderer : public ObjectRenderer
{
public:

	void render() override
	{	}
};

#endif // POINT_RENDERER_H
