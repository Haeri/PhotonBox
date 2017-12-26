#ifndef DEFERRED_BUFFER_H
#define DEFERRED_BUFFER_H

#include "../Core/FrameBuffer.h"

class DeferredBuffer {
public:
	//unsigned int gBuffer;
	//unsigned int gPosition, gNormal, gAlbedoSpec;
	//unsigned int rboDepth;

	FrameBuffer* gBuffer;

	void init();
};

#endif // DEFERRED_BUFFER_H