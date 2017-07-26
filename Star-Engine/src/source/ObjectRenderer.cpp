#include "../header/ObjectRenderer.h"
#include "../header/Core.h"

ObjectRenderer::ObjectRenderer() {
	Core::getInstance()->renderer.addToRenderQueue(this);
}

void ObjectRenderer::onDestroy() {}

void ObjectRenderer::destroy(){
	onDestroy();
	Core::getInstance()->renderer.removeFromRenderQueue(this);
}
