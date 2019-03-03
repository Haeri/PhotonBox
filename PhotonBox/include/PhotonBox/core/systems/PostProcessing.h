#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

class PostProcessor;

#include <map>
#include <set>

class PostProcessing
{
public:
	static void addProcessor(PostProcessor* processor);
	static void removeProcessor(PostProcessor* processor);
	static bool isActive();
	static void resizeAll();
	void start();
	void postProcess();
	void reset();
	void destroy();

	void drawGizmos();
private:
	static bool shouldPostProcess();
	static std::map<int, PostProcessor*> _processorMap;
	static std::set<PostProcessor*> _processorResourceSet;
};

#endif // POST_PROCESSING_H
