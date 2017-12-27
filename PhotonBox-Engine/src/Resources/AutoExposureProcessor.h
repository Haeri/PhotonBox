#ifndef AUTO_EXPOSURE_PROCESSOR_H
#define AUTO_EXPOSURE_PROCESSOR_H

#include "../Core/PostProcessor.h"
#include "../Game/MonochromShader.h"
#include "../Core/FrameBuffer.h"
#include "ToneMappingShader.h"
#include "AutoExposureShader.h"
#include "Material.h"
#include "Exposure.h"

class AutoExposureProcessor : public PostProcessor {
public:
	float minLuminance = 0.01;
	float maxLuminance = 100;


	AutoExposureProcessor(int index) : PostProcessor(index) {
		int res = 64;
		int numLevels = 1 + floor(log2(res));

		autoExpMaterial = new Material(AutoExposureShader::getInstance());
		expMaterial = new Material(ExposureShader::getInstance());
		
		currentLuminancBuffer = new FrameBuffer(res, res);
		currentLuminancBuffer->addTextureAttachment("color", false, true, true);
		luminancBufferA = new FrameBuffer(1, 1);
		luminancBufferA->addTextureAttachment("color", false, true);
		luminancBufferB = new FrameBuffer(1, 1);
		luminancBufferB->addTextureAttachment("color", false, true);
		mainBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight());
		mainBuffer->addTextureAttachment("collor", false, true);

		autoExpMaterial->setTexture("luminanceSampleCurrent", currentLuminancBuffer);
		autoExpMaterial->setProperty<int>("maxMip", numLevels);
		autoExpMaterial->setProperty<float>("adaptationSpeed", 0.4);

		expMaterial->setTexture("renderTexture", mainBuffer);
	}

	void enable() override {
		mainBuffer->enable();
	}

	void preProcess() override {
		autoExpMaterial->setProperty<float>("minLum", minLuminance);
		autoExpMaterial->setProperty<float>("maxLum", maxLuminance);

		currentLuminancBuffer->enable();
		mainBuffer->render("color");

		if (flip) {
			luminancBufferA->enable();
			autoExpMaterial->setTexture("luminanceSampleLast", luminancBufferB);
		}else {
			luminancBufferB->enable();
			autoExpMaterial->setTexture("luminanceSampleLast", luminancBufferB);
		}
		currentLuminancBuffer->render("color", autoExpMaterial);
	}

	void render() override {
		if (flip) {
			expMaterial->setTexture("exposureSample", luminancBufferA);
		}else {
			expMaterial->setTexture("exposureSample", luminancBufferB);
		}
		mainBuffer->render("color", expMaterial);
		flip = !flip;
	}

	void destroy() override {
		delete autoExpMaterial;

		mainBuffer->destroy();
		currentLuminancBuffer->destroy();
		delete mainBuffer;
		delete currentLuminancBuffer;
	}
private:
	Material* autoExpMaterial, *expMaterial;
	FrameBuffer* mainBuffer, *currentLuminancBuffer, *luminancBufferA, *luminancBufferB;
	bool flip;
};

#endif // AUTO_EXPOSURE_PROCESSOR_H