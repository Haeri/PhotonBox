#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

#include "FrameBuffer.h"

class PostProcessing {
public:
	void init();
	void preProcess();
	void postProcess();
	void destroy();
private:
	FrameBuffer* mono;
	Material* normal;
};

#endif /* defined(POST_PROCESSING_H) */
