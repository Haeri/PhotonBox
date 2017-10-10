#ifndef MONOCHROM_PROCESSOR_H
#define MONOCHROM_PROCESSOR_H

#include "../Core/PostProcessor.h"

class MonochromProcessor: public PostProcessor {
public:
	MonochromProcessor(int index, Material* material): PostProcessor(index, material){}

	void render() override {
		_frameBuffer->render(material);
	}
};

#endif // MONOCHROM_PROCESSOR_H