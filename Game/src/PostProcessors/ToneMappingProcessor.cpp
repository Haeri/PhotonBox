#ifndef TONE_MAPPING_PROCESSOR_CPP
#define TONE_MAPPING_PROCESSOR_CPP

#include <resource/PostProcessor.h>
#include <resource/Material.h>
#include <resource/FrameBuffer.h>

#include "../Shader/ToneMappingShader.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class ToneMappingProcessor : public PostProcessor
{
public:
	ToneMappingProcessor(int index) : PostProcessor(index)
	{
		_material = new Material(ToneMappingShader::getInstance());
		_material->setImageBuffer("renderTexture", mainBuffer->getAttachment("color"));
	}

	void render(FrameBuffer* nextBuffer) override
	{
		nextBuffer->enable();
		mainBuffer->render(_material);
	}

	void destroy() override
	{
		delete _material;
	}
private:
	Material * _material;
};

#endif // TONE_MAPPING_PROCESSOR_CPP