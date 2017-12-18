#ifndef SSAO_PROCESSOR_H
#define SSAO_PROCESSOR_H

#include "../Core/PostProcessor.h"
#include "../Game/MonochromShader.h"
#include "../Core/FrameBuffer.h"
#include "ToneMappingShader.h"
#include "AutoExposureShader.h"
#include "Material.h"
#include "Exposure.h"

class SSAOProcessor : public PostProcessor {
public:

	SSAOProcessor(int index) : PostProcessor(index) {
		mainBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight(), false);
	}

	void enable() override {
		mainBuffer->enable();
	}

	void preProcess() override {

	}

	void render() override {

	}

	void destroy() override {

	}
private:
	Material * autoExpMaterial, *expMaterial;
	FrameBuffer* mainBuffer, *currentLuminancBuffer, *luminancBufferA, *luminancBufferB;
	bool flip;
};

#endif // SSAO_PROCESSOR_H