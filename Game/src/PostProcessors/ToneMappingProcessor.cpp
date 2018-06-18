#ifndef TONE_MAPPING_PROCESSOR_CPP
#define TONE_MAPPING_PROCESSOR_CPP

#include <Core/FrameBuffer.h>
#include <Core/PostProcessor.h>

#include "../Shader/MonochromShader.cpp"
#include "../Shader/ToneMappingShader.cpp"

class ToneMappingProcessor : public PostProcessor
{
public:
	ToneMappingProcessor(int index) : PostProcessor(index)
	{
		_material = new Material(ToneMappingShader::getInstance());
		_frameBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight());
		_frameBuffer->addTextureAttachment("color", true);
		_frameBuffer->ready();

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