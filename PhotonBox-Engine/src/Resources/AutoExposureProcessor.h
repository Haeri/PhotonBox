#ifndef AUTO_EXPOSURE_PROCESSOR_H
#define AUTO_EXPOSURE_PROCESSOR_H

#include "../Core/PostProcessor.h"
#include "../Game/MonochromShader.h"
#include "../Core/FrameBuffer.h"
#include "ToneMappingShader.h"
#include "AutoExposureShader.h"
#include "Material.h"

class AutoExposureProcessor : public PostProcessor {
public:
	Material* material;
	FrameBuffer* mainBuffer, *luminancBufferA, *luminancBufferB;

	AutoExposureProcessor(int index) : PostProcessor(index) {
		int res = 64;
		int numLevels = 1 + floor(log2(res));

		//int numLevels = 1+floor(log2(min(Display::getWidth(), Display::getHeight())));
		//float res = pow(2, numLevels-1);

		material = new Material(AutoExposureShader::getInstance());
		luminancBufferA = new FrameBuffer(res, res, true);
		luminancBufferB = new FrameBuffer(res, res, true);
		mainBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight(), true);

		material->setTexture("renderTexture", mainBuffer);
		material->setProperty<int>("maxMip", numLevels);
		material->setProperty<float>("adaptationSpeed", 0.00001);
	}

	void enable() override {
		mainBuffer->enable();
	}

	void preProcess() override {
		if (flip) {
			material->setTexture("luminanceSampleCurrent", luminancBufferA);
			material->setTexture("luminanceSampleLast", luminancBufferB);
			luminancBufferA->enable();
			mainBuffer->render();
		}else {
			material->setTexture("luminanceSampleCurrent", luminancBufferB);
			material->setTexture("luminanceSampleLast", luminancBufferA);
			luminancBufferB->enable();
			mainBuffer->render();
		}
		flip = !flip;
	}

	void render() override {
		mainBuffer->render(material);
	}

	void destroy() override {
		delete material;

		mainBuffer->destroy();
		luminancBufferA->destroy();
		delete mainBuffer;
		delete luminancBufferA;
	}
private:
	bool flip;
};

#endif // AUTO_EXPOSURE_PROCESSOR_H