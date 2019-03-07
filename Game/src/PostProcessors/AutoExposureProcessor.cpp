#ifndef AUTO_EXPOSURE_PROCESSOR_CPP
#define AUTO_EXPOSURE_PROCESSOR_CPP

#include <resource/PostProcessor.h>
#include <resource/Material.h>

#include "../Shader/ToneMappingShader.cpp"
#include "../Shader/MonochromShader.cpp"
#include "../Shader/AutoExposureShader.cpp"
#include "../Shader/ExposureShader.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class AutoExposureProcessor : public PostProcessor
{
public:
	float minLuminance = 0.5;
	float maxLuminance = 10;
	float adaptationSpeed = 3;

	AutoExposureProcessor(int index) : PostProcessor(index)
	{
		int res = 64;
		int numLevels = 1 + (int)floor(log2(res));

		autoExpMaterial = new Material(AutoExposureShader::getInstance());
		expMaterial = new Material(ExposureShader::getInstance());

		currentLuminancBuffer = new FrameBuffer(res, res);
		currentLuminancBuffer->addTextureAttachment("color", true, true);
		currentLuminancBuffer->ready();
		luminancBufferA = new FrameBuffer(1, 1);
		luminancBufferA->addTextureAttachment("color", true);
		luminancBufferA->ready();
		luminancBufferB = new FrameBuffer(1, 1);
		luminancBufferB->addTextureAttachment("color", true);
		luminancBufferB->ready();
		
		autoExpMaterial->setTexture("luminanceSampleCurrent", currentLuminancBuffer, "color");
		autoExpMaterial->setProperty<int>("maxMip", numLevels);
		autoExpMaterial->setProperty<float>("adaptationSpeed", adaptationSpeed);
		autoExpMaterial->setProperty<float>("minLum", minLuminance);
		autoExpMaterial->setProperty<float>("maxLum", maxLuminance);

		expMaterial->setTexture("renderTexture", mainBuffer, "color");
	}

	void render(FrameBuffer* nextBuffer) override
	{
		// Prepare
		currentLuminancBuffer->enable();
		mainBuffer->render("color");

		if (_flip)
		{
			luminancBufferA->enable();
			autoExpMaterial->setTexture("luminanceSampleLast", luminancBufferB, "color");
		}
		else
		{
			luminancBufferB->enable();
			autoExpMaterial->setTexture("luminanceSampleLast", luminancBufferB, "color");
		}
		currentLuminancBuffer->render(autoExpMaterial);
	
		// Render
		nextBuffer->enable();
		if (_flip)
		{
			expMaterial->setTexture("exposureSample", luminancBufferA, "color");
		}
		else
		{
			expMaterial->setTexture("exposureSample", luminancBufferB, "color");
		}
		mainBuffer->render(expMaterial);
		_flip = !_flip;
	}

	void destroy() override
	{
		delete autoExpMaterial;
		delete currentLuminancBuffer;
		delete luminancBufferA;
		delete luminancBufferB;
		delete expMaterial;
	}
private:
	Material * autoExpMaterial, *expMaterial;
	FrameBuffer *currentLuminancBuffer, *luminancBufferA, *luminancBufferB;
	bool _flip = false;
};

#endif // AUTO_EXPOSURE_PROCESSOR_CPP