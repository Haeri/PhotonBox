#ifndef DEFERRED_BUFFER_H
#define DEFERRED_BUFFER_H

class DeferredBuffer {
public:
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec;
	unsigned int rboDepth;

	void init();
};

#endif // DEFERRED_BUFFER_H