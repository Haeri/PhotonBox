#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

class PostProcessor;
#include <map>
#include "../../Core/FrameBuffer.h"

class PostProcessing {
public:
	static void addProcessor(PostProcessor* processor);
	static void removeProcessor(PostProcessor* processor);
	static bool isActive() { return _doPostProcessing; }
	void start();
	void postProcess();
	void destroy();
private:
	static bool _doPostProcessing;
	static std::map<int, PostProcessor*> _processorMap;
};

#endif /* defined(POST_PROCESSING_H) */
