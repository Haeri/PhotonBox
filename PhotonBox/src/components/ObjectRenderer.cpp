#include "PhotonBox/components/ObjectRenderer.h"

#include "PhotonBox/core/systems/Renderer.h"

ObjectRenderer::ObjectRenderer(RenderType type)
{
	_type = type;
	Renderer::addToRenderQueue(this, type);
}

void ObjectRenderer::init() {}
void ObjectRenderer::onDestroy() {}

BoundingSphere ObjectRenderer::getBoundingSphere()
{
	return BoundingSphere(Vector3f::ZERO, 0.001f);
}

void ObjectRenderer::destroy()
{
	onDestroy();
	Renderer::removeFromRenderQueue(this);
}

void ObjectRenderer::setRenderType(RenderType type)
{
	Renderer::removeFromRenderQueue(this);
	Renderer::addToRenderQueue(this, type);
	_type = type;
}
