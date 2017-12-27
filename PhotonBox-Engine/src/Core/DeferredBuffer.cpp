#include "DeferredBuffer.h"
#include "../Core/FrameBuffer.h"

void DeferredBuffer::init()
{
	int width = Display::getWidth();
	int height = Display::getHeight();

	gBuffer = new FrameBuffer(width, width);
	gBuffer->addTextureAttachment("gPosition", true);
	gBuffer->addTextureAttachment("gNormal", true);
	gBuffer->addDepthBufferAttachment();
	gBuffer->ready();
}
