#include "DeferredBuffer.h"
#include "../Core/FrameBuffer.h"

void DeferredBuffer::init()
{
	int width = Display::getWidth();
	int height = Display::getHeight();

	gBuffer = new FrameBuffer(width, height);
	gBuffer->addTextureAttachment("gPosition", true);
	gBuffer->addTextureAttachment("gNormal", true);
	gBuffer->addTextureAttachment("gMetallic");
	gBuffer->addTextureAttachment("gRoughness");
	gBuffer->addTextureAttachment("gAlbedo");
	gBuffer->addDepthBufferAttachment();
	gBuffer->ready();
}
