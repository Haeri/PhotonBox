#ifndef TONE_MAPPING_PROCESSOR_H
#define TONE_MAPPING_PROCESSOR_H

#include "../Core/FrameBuffer.h"
#include "../Core/PostProcessor.h"
#include "../Game/MonochromShader.h"
#include "ToneMappingShader.h"

class ToneMappingProcessor : public PostProcessor
{
public:
	ToneMappingProcessor(int index) : PostProcessor(index)
	{
		_material = new Material(ToneMappingShader::getInstance());
		_frameBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight());
		_frameBuffer->addTextureAttachment("color", true);
		_frameBuffer->ready();

		_material->setProperty<float>("exposure", 0.2f);
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

#endif // TONE_MAPPING_PROCESSOR_H
