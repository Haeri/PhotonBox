#ifndef MONOCHROM_PROCESSOR_CPP
#define MONOCHROM_PROCESSOR_CPP

#include <resource/PostProcessor.h>
#include <resource/Material.h>

#include "../Shader/MonochromShader.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class MonochromProcessor : public PostProcessor
{
public:
	Material * _material;

	MonochromProcessor(int index) : PostProcessor(index)
	{
		_material = new Material(MonochromShader::getInstance());
		_material->setTexture("renderTexture", mainBuffer, "color");
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
};

#endif // MONOCHROM_PROCESSOR_CPP