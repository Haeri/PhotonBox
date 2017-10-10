#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

class Material;
#include "PostProcessing.h"

class PostProcessor {
public:
	Material* material;

	PostProcessor(int index, Material* material) {
		this->material = material;
		_index = index;
		PostProcessing::addProcessor(this);
	}

	void setFrameBuffer(FrameBuffer* frameBuffer) {
		_frameBuffer = frameBuffer;
	}
	
	int getIndex() { return _index; }
	
	void enable() {
		_frameBuffer->enable();
	}

	void destroy() {
		delete _frameBuffer;
		delete material;
	}

	virtual void init() {}
	virtual void render() = 0;
protected:
	int _index;
	FrameBuffer* _frameBuffer;
};

#endif // POST_PROCESSOR_H