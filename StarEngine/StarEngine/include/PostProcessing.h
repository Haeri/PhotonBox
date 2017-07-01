#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

#include "Renderer.h"
#include "Core.h"

class PostProcessing {
public:
	static void init();
	static void preProcess();
	static void postProcess();
	
private:
};

#endif /* defined(POST_PROCESSING_H) */
