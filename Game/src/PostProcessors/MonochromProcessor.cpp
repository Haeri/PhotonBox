#ifndef MONOCHROM_PROCESSOR_CPP
#define MONOCHROM_PROCESSOR_CPP

#include <Core/FrameBuffer.h>
#include <Core/PostProcessor.h>
#include <Resources/Material.h>

#include "../Shader/MonochromShader.cpp"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class MonochromProcessor : public PostProcessor
{
public:
	Material * _material;
	FrameBuffer* _frameBuffer;

	MonochromProcessor(int index) : PostProcessor(index)
	{
		_frameBuffer = new FrameBuffer(1);
		_frameBuffer->addTextureAttachment("color", true);
		_frameBuffer->ready();

		_material = new Material(MonochromShader::getInstance());
		_material->setTexture("renderTexture", _frameBuffer, "color");
	}

	void prepare() override
	{
		_frameBuffer->enable();
	}

	void render() override
	{
		_frameBuffer->render(_material);
	}

	void destroy() override
	{
		delete _material;
		delete _frameBuffer;
	}
};

#endif // MONOCHROM_PROCESSOR_CPP