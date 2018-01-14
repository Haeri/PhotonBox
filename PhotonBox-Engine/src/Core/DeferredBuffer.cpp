#include "DeferredBuffer.h"
#include "FrameBuffer.h"

void DeferredBuffer::init()
{
	gBuffer = new FrameBuffer(1);
	gBuffer->addTextureAttachment("gPosition", true);
	gBuffer->addTextureAttachment("gNormal", true);
	gBuffer->addTextureAttachment("gMetallic");
	gBuffer->addTextureAttachment("gRoughness");
	//gBuffer->addTextureAttachment("gAlbedo");
	gBuffer->addDepthBufferAttachment();
	gBuffer->ready();
}
