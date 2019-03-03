#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

class PostProcessor;

#include <map>
#include <set>

#include "PhotonBox/core/System.h"

class PostProcessing : public System
{
public:
	static void addProcessor(PostProcessor* processor);
	static void removeProcessor(PostProcessor* processor);
	static bool isActive();
	static void resizeAll();
	
	void start() override;
	void reset() override;
	void destroy() override;

	void postProcess();
	void drawGizmos();
private:
	static bool shouldPostProcess();
	static std::map<int, PostProcessor*> _processorMap;
	static std::set<PostProcessor*> _processorResourceSet;
};

#endif // POST_PROCESSING_H
