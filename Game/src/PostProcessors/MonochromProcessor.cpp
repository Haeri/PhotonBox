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
	Material * material;
	FrameBuffer* frameBuffer;

	MonochromProcessor(int index) : PostProcessor(index)
	{
		material = new Material(MonochromShader::getInstance());
		frameBuffer = new FrameBuffer(1);
	}

	void enable() override
	{
		frameBuffer->enable();
	}

	void render() override
	{
		frameBuffer->render(material);
	}

	void destroy() override
	{
		delete material;
		delete frameBuffer;
	}

};

#endif // MONOCHROM_PROCESSOR_CPP