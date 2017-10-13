#ifndef MONOCHROM_PROCESSOR_H
#define MONOCHROM_PROCESSOR_H

#include "../Core/PostProcessor.h"

class MonochromProcessor: public PostProcessor {
public:
	Material* material;
	FrameBuffer* frameBuffer;

	MonochromProcessor(int index, Material* material): PostProcessor(index) {
		this->material = material;
		frameBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight());
	}

	void enable() override {
		frameBuffer->enable();
	}

	void render() override {
		frameBuffer->render(material);
	}

	void destroy() override {
		delete material;
		delete frameBuffer;
	}

};

#endif // MONOCHROM_PROCESSOR_H