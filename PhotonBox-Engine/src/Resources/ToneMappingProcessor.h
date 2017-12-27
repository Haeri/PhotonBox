#ifndef TONE_MAPPING_PROCESSOR_H
#define TONE_MAPPING_PROCESSOR_H

#include "../Core/PostProcessor.h"
#include "../Game/MonochromShader.h"
#include "../Core/FrameBuffer.h"
#include "ToneMappingShader.h"

class ToneMappingProcessor : public PostProcessor {
public:
	Material* material;
	FrameBuffer* frameBuffer;

	ToneMappingProcessor(int index) : PostProcessor(index) {
		material = new Material(ToneMappingShader::getInstance());
		frameBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight());
		frameBuffer->addTextureAttachment("color", true);
		frameBuffer->ready();

		material->setProperty<float>("exposure", 0.2f);
		material->setTexture("renderTexture", frameBuffer, "color");
	}

	void enable() override {
		frameBuffer->enable();
	}

	void render() override {
		frameBuffer->render("color", material);
	}

	void destroy() override {
		delete material;
		delete frameBuffer;
	}

};

#endif // TONE_MAPPING_PROCESSOR_H