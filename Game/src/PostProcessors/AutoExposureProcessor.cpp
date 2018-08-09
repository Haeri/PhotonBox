#ifndef AUTO_EXPOSURE_PROCESSOR_CPP
#define AUTO_EXPOSURE_PROCESSOR_CPP

#include <core/FrameBuffer.h>
#include <core/PostProcessor.h>
#include <resources/Material.h>

#include "../Shader/ToneMappingShader.cpp"
#include "../Shader/MonochromShader.cpp"
#include "../Shader/AutoExposureShader.cpp"
#include "../Shader/ExposureShader.cpp"

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
		mainBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight());
		mainBuffer->addTextureAttachment("color", true);
		mainBuffer->ready();

		autoExpMaterial->setTexture("luminanceSampleCurrent", currentLuminancBuffer, "color");
		autoExpMaterial->setProperty<int>("maxMip", numLevels);
		autoExpMaterial->setProperty<float>("adaptationSpeed", adaptationSpeed);
		autoExpMaterial->setProperty<float>("minLum", minLuminance);
		autoExpMaterial->setProperty<float>("maxLum", maxLuminance);

		expMaterial->setTexture("renderTexture", mainBuffer, "color");
	}

	void enable() override
	{
		mainBuffer->enable();
	}

	void preProcess() override
	{
		currentLuminancBuffer->enable();
		mainBuffer->render("color");

		if (flip)
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
	}

	void render() override
	{
		if (flip)
		{
			expMaterial->setTexture("exposureSample", luminancBufferA, "color");
		}
		else
		{
			expMaterial->setTexture("exposureSample", luminancBufferB, "color");
		}
		mainBuffer->render(expMaterial);
		flip = !flip;

		/*
		Renderer::_debugFrameBuffer->enable();

		int cols = 4;
		int widthX = 0;

		glViewport(widthX, 0, Display::getWidth() / cols, Display::getHeight() / cols);
		if (flip)
			luminancBufferA->render("color");
		else
			luminancBufferB->render("color");
		widthX += Display::getWidth() / cols;

		glViewport(widthX, 0, Display::getWidth() / cols, Display::getHeight() / cols);
		if (flip)
			luminancBufferB->render("color");
		else
			luminancBufferA->render("color");
		widthX += Display::getWidth() / cols;
		*/
	}

	void destroy() override
	{
		delete autoExpMaterial;
		delete mainBuffer;
		delete currentLuminancBuffer;
		delete luminancBufferA;
		delete luminancBufferB;
	}
private:
	Material * autoExpMaterial, *expMaterial;
	FrameBuffer* mainBuffer, *currentLuminancBuffer, *luminancBufferA, *luminancBufferB;
	bool flip;
};

#endif // AUTO_EXPOSURE_PROCESSOR_CPP