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
	Material* autoExpMaterial, *expMaterial;
	FrameBuffer* mainBuffer, *currentLuminancBuffer, *luminancBufferA, *luminancBufferB;

	AutoExposureProcessor(int index) : PostProcessor(index) {
		int res = 64;
		int numLevels = 1 + floor(log2(res));

		autoExpMaterial = new Material(AutoExposureShader::getInstance());
		expMaterial = new Material(ExposureShader::getInstance());
		
		currentLuminancBuffer = new FrameBuffer(res, res, true);
		luminancBufferA = new FrameBuffer(1, 1);
		luminancBufferB = new FrameBuffer(1, 1);
		
		mainBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight(), false);

		autoExpMaterial->setTexture("luminanceSampleCurrent", currentLuminancBuffer);
		autoExpMaterial->setProperty<int>("maxMip", numLevels);
		autoExpMaterial->setProperty<float>("adaptationSpeed", 0.4);

		expMaterial->setTexture("renderTexture", mainBuffer);
	}

	void enable() override {
		mainBuffer->enable();
	}

	void preProcess() override {
		currentLuminancBuffer->enable();
		mainBuffer->render();

		if (flip) {
			luminancBufferA->enable();
			autoExpMaterial->setTexture("luminanceSampleLast", luminancBufferB);
		}else {
			luminancBufferB->enable();
			autoExpMaterial->setTexture("luminanceSampleLast", luminancBufferB);
		}
		currentLuminancBuffer->render(autoExpMaterial);
	}

	void render() override {
		if (flip) {
			expMaterial->setTexture("exposureSample", luminancBufferA);
		}else {
			expMaterial->setTexture("exposureSample", luminancBufferB);
		}
		mainBuffer->render(expMaterial);
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
	bool flip;
};

#endif // AUTO_EXPOSURE_PROCESSOR_H