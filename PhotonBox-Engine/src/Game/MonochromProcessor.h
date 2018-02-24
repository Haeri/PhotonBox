#ifndef MONOCHROM_PROCESSOR_H
#define MONOCHROM_PROCESSOR_H

#include "../Core/FrameBuffer.h"
#include "../Core/PostProcessor.h"
#include "../Resources/Material.h"
#include "MonochromShader.h"

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

#endif // MONOCHROM_PROCESSOR_H
