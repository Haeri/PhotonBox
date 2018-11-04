#ifndef TONE_MAPPING_PROCESSOR_CPP
#define TONE_MAPPING_PROCESSOR_CPP

#include <Core/PostProcessor.h>

#include "../Shader/ToneMappingShader.cpp"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class ToneMappingProcessor : public PostProcessor
{
public:
	ToneMappingProcessor(int index) : PostProcessor(index)
	{
		_material = new Material(ToneMappingShader::getInstance());
		_frameBuffer = new FrameBuffer(1);
		_frameBuffer->addTextureAttachment("color", true);
		_frameBuffer->ready();

		_material = new Material(ToneMappingShader::getInstance());
		_material->setTexture("renderTexture", _frameBuffer, "color");
	}

	void enable() override
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
private:
	Material * _material;
	FrameBuffer* _frameBuffer;
};

#endif // TONE_MAPPING_PROCESSOR_CPP