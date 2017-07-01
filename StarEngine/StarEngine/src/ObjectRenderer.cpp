#include "../include/ObjectRenderer.h"
#include "../include/Renderer.h"

ObjectRenderer::ObjectRenderer() {
	Renderer::addToRenderQueue(this);
}

void ObjectRenderer::onDestroy() {}

void ObjectRenderer::destroy(){
	onDestroy();
	Renderer::removeFromRenderQueue(this);
}
