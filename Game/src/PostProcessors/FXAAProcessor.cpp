#ifndef FXAA_PROCESSOR_CPP
#define FXAA_PROCESSOR_CPP

#include <Core/FrameBuffer.h>
#include <Core/PostProcessor.h>
#include <Resources/Material.h>
#include <core/Display.h>
#include <resources/MedianShader.h>

#include "../Shader/FXAAShader.cpp"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class FXAAProcessor : public PostProcessor
{
public:
	Material * _fxaaMaterial;
	Material * _medianMaterial;
	FrameBuffer* _frameBuffer;
	FrameBuffer* _tempBuffer;

	FXAAProcessor(int index) : PostProcessor(index)
	{
		_frameBuffer = new FrameBuffer(1);
		_frameBuffer->addTextureAttachment("color", true);
		_frameBuffer->ready();

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
		_medianMaterial->setTexture("renderTexture", _frameBuffer, "color");
	}

	void onResize() override
	{
		_fxaaMaterial->setProperty<float>("screenWidth", Display::getWidth());
		_fxaaMaterial->setProperty<float>("screenHeight", Display::getHeight());
	}

	void prepare() override
	{
		_frameBuffer->enable();
	}

	void preProcess() override
	{
		_tempBuffer->enable();
		_frameBuffer->render(_medianMaterial);
	}

	void render() override
	{
		_tempBuffer->render(_fxaaMaterial);
	}

	void destroy() override
	{
		delete _fxaaMaterial;
		delete _medianMaterial;
		delete _frameBuffer;
		delete _tempBuffer;
	}

};

#endif // FXAA_PROCESSOR_CPP