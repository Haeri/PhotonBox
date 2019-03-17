#include "PhotonBox/component/ObjectRenderer.h"

#include "PhotonBox/core/system/Renderer.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void ObjectRenderer::init()
{
	Renderer::addToRenderQueue(this, _type);
}

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
