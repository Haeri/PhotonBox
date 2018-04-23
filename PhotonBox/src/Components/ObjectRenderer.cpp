#include "../Core/Systems/Renderer.h"
#include "ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(RenderType type)
{
	_type = type;
	Renderer::addToRenderQueue(this, type);
}

void ObjectRenderer::init() {}
void ObjectRenderer::onDestroy() {}

BoundingSphere ObjectRenderer::getBoundingSphere()
{
	return BoundingSphere(Vector3f::ZERO, 0.001);
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
