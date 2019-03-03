#ifndef FXAA_PROCESSOR_CPP
#define FXAA_PROCESSOR_CPP

#include <resource/PostProcessor.h>
#include <resource/Material.h>
#include <core/Display.h>
#include <resource/shader/MedianShader.h>

#include "../Shader/FXAAShader.cpp"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class FXAAProcessor : public PostProcessor
{
public:

	FXAAProcessor(int index) : PostProcessor(index)
	{
		_tempBuffer = new FrameBuffer(1);
		_tempBuffer->addTextureAttachment("color", true);
		_tempBuffer->ready();

		_fxaaMaterial = new Material(FXAAShader::getInstance());
		_fxaaMaterial->setProperty<float>("R_fxaaSpanMax", 8.0f);
		_fxaaMaterial->setProperty<float>("R_fxaaReduceMin", 1.0f / 128.0f);
		_fxaaMaterial->setProperty<float>("R_fxaaReduceMul", 150.0f);
		_fxaaMaterial->setProperty<float>("screenWidth", Display::getWidth());
		_fxaaMaterial->setProperty<float>("screenHeight", Display::getHeight());
		_fxaaMaterial->setTexture("renderTexture", _tempBuffer, "color");

		_medianMaterial = new Material(MedianShader::getInstance());
		_medianMaterial->setTexture("renderTexture", mainBuffer, "color");
	}

	void onResize() override
	{
		_fxaaMaterial->setProperty<float>("screenWidth", Display::getWidth());
		_fxaaMaterial->setProperty<float>("screenHeight", Display::getHeight());
	}

	void render(FrameBuffer* nextBuffer) override
	{
		// Prepare
		_tempBuffer->enable();
		mainBuffer->render(_medianMaterial);

		// Render
		nextBuffer->enable();
		_tempBuffer->render(_fxaaMaterial);
	}

	void destroy() override
	{
		delete _fxaaMaterial;
		delete _medianMaterial;
		delete _tempBuffer;
	}

private:
	Material * _fxaaMaterial;
	Material * _medianMaterial;
	FrameBuffer* _tempBuffer;
};

#endif // FXAA_PROCESSOR_CPP