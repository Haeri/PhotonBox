#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

class Material;
#include "PhotonBox/core/systems/PostProcessing.h"

class PostProcessor
{
public:
	PostProcessor(int index)
	{
		_index = index;
		PostProcessing::addProcessor(this);
	}

	int getIndex() { return _index; }

	virtual void onResize() {}
	virtual void enable() = 0;
	virtual void preProcess() {}
	virtual void render() = 0;
	virtual	void destroy() = 0;
protected:
	int _index;
};

#endif // POST_PROCESSOR_H
