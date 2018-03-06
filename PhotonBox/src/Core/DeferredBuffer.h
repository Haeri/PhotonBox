#ifndef DEFERRED_BUFFER_H
#define DEFERRED_BUFFER_H

class FrameBuffer;

class DeferredBuffer
{
public:
	FrameBuffer * gBuffer;

	void init();
};

#endif // DEFERRED_BUFFER_H
