#ifndef FXAA_PROCESSOR_CPP
#define FXAA_PROCESSOR_CPP

#include <Core/FrameBuffer.h>
#include <Core/PostProcessor.h>
#include <Resources/Material.h>
#include <core/Display.h>

#include "../Shader/FXAAShader.cpp"

class FXAAProcessor : public PostProcessor
{
public:
	Material * _material;
	FrameBuffer* _frameBuffer;

	FXAAProcessor(int index) : PostProcessor(index)
	{
		_frameBuffer = new FrameBuffer(1);
		_frameBuffer->addTextureAttachment("color", true);
		_frameBuffer->ready();

		_material = new Material(FXAAShader::getInstance());
		_material->setProperty<float>("R_fxaaSpanMax", 8.0f);
		_material->setProperty<float>("R_fxaaReduceMin", 1.0f / 128.0f);
		_material->setProperty<float>("R_fxaaReduceMul", 150.0f);
		_material->setProperty<float>("screenWidth", Display::getWidth());
		_material->setProperty<float>("screenHeight", Display::getHeight());
		_material->setTexture("renderTexture", _frameBuffer, "color");
	}

	void onResize() override
	{
		_material->setProperty<float>("screenWidth", Display::getWidth());
		_material->setProperty<float>("screenHeight", Display::getHeight());
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

};

#endif // FXAA_PROCESSOR_CPP