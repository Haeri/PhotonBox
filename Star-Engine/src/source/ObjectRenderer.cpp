#include "../header/ObjectRenderer.h"
#include "../header/Renderer.h"

ObjectRenderer::ObjectRenderer() {
	Renderer::addToRenderQueue(this);
}

void ObjectRenderer::onDestroy() {}

void ObjectRenderer::destroy(){
	onDestroy();
	Renderer::removeFromRenderQueue(this);
}
