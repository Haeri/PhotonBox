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
	FrameBuffer* mainBuffer, *luminancBuffer;

	AutoExposureProcessor(int index) : PostProcessor(index) {
		int numLevels = 1+floor(log2(min(Display::getWidth(), Display::getHeight())));
		float res = pow(2, numLevels-1);

		material = new Material(AutoExposureShader::getInstance());
		luminancBuffer = new FrameBuffer(res, res, true);
		mainBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight(), true);

		material->setTexture("renderTexture", mainBuffer);
		material->setTexture("luminanceSample", luminancBuffer);
		material->setProperty<int>("maxMip", numLevels);
	}

	void enable() override {
		mainBuffer->enable();
	}

	void preProcess() override {
		luminancBuffer->enable();
		mainBuffer->render();
	}

	void render() override {
		mainBuffer->render(material);
	}

	void destroy() override {
		delete material;

		mainBuffer->destroy();
		luminancBuffer->destroy();
		delete mainBuffer;
		delete luminancBuffer;
	}

};

#endif // AUTO_EXPOSURE_PROCESSOR_H