#include "ObjectRenderer.h"
#include "TheRenderer.h"

ObjectRenderer::ObjectRenderer() {
	TheRenderer::addToRenderQueue(this);
}