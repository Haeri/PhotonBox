#include "../Core/Systems/Renderer.h"
#include "ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(bool isOpaque)
{
	_isOpaque = isOpaque;
	Renderer::addToRenderQueue(this, isOpaque);
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
