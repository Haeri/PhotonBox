#include "ObjectRenderer.h"
#include "../Core/Systems/Renderer.h"

ObjectRenderer::ObjectRenderer() {
	Renderer::addToRenderQueue(this);
}

void ObjectRenderer::init() {}
void ObjectRenderer::onDestroy() {}

void ObjectRenderer::destroy(){
	onDestroy();
	Renderer::removeFromRenderQueue(this);
}
