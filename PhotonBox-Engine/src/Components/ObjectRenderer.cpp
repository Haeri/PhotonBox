#include "ObjectRenderer.h"
#include "../Core/Systems/Renderer.h"

ObjectRenderer::ObjectRenderer(bool isOpaque)
{
	_isOpaque = isOpaque;
	Renderer::addToRenderQueue(this, isOpaque);
}

void ObjectRenderer::init() {}
void ObjectRenderer::onDestroy() {}

void ObjectRenderer::destroy()
{
	onDestroy();
	Renderer::removeFromRenderQueue(this);
}
