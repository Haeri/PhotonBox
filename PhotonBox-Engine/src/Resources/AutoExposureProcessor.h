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
	float maxLuminance = 20;


	AutoExposureProcessor(int index) : PostProcessor(index) {
		int res = 64;
		int numLevels = 1 + floor(log2(res));

		autoExpMaterial = new Material(AutoExposureShader::getInstance());
		expMaterial = new Material(ExposureShader::getInstance());
		
		currentLuminancBuffer = new FrameBuffer(res, res);
		currentLuminancBuffer->addTextureAttachment("color", false, true);
		currentLuminancBuffer->ready();
		luminancBufferA = new FrameBuffer(1, 1);
		luminancBufferA->addTextureAttachment("color");
		luminancBufferA->ready();
		luminancBufferB = new FrameBuffer(1, 1);
		luminancBufferB->addTextureAttachment("color");
		luminancBufferB->ready();
		mainBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight());
		mainBuffer->addTextureAttachment("color", true);
		mainBuffer->ready();

		autoExpMaterial->setTexture("luminanceSampleCurrent", currentLuminancBuffer, "color");
		autoExpMaterial->setProperty<int>("maxMip", numLevels);
		autoExpMaterial->setProperty<float>("adaptationSpeed", 0.4);

		expMaterial->setTexture("renderTexture", mainBuffer, "color");
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
			autoExpMaterial->setTexture("luminanceSampleLast", luminancBufferB, "color");
		}else {
			luminancBufferB->enable();
			autoExpMaterial->setTexture("luminanceSampleLast", luminancBufferB, "color");
		}
		currentLuminancBuffer->render("color", autoExpMaterial);
	}

	void render() override {
		if (flip) {
			expMaterial->setTexture("exposureSample", luminancBufferA, "color");
		}else {
			expMaterial->setTexture("exposureSample", luminancBufferB, "color");
		}
		mainBuffer->render("color", expMaterial);
		flip = !flip;
	}

	void destroy() override {
		delete autoExpMaterial;
		delete mainBuffer;
		delete currentLuminancBuffer;
		delete luminancBufferA;
		delete luminancBufferB;
	}
private:
	Material* autoExpMaterial, *expMaterial;
	FrameBuffer* mainBuffer, *currentLuminancBuffer, *luminancBufferA, *luminancBufferB;
	bool flip;
};

#endif // AUTO_EXPOSURE_PROCESSOR_H