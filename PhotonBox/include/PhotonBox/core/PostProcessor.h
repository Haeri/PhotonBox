#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

class Material;

#include "PhotonBox/core/systems/PostProcessing.h"

class PostProcessor
{
public:
	PostProcessor(int index);
	virtual ~PostProcessor() {}

	int getIndex();
	bool isEnabled();
	void setEnabled(bool enable);

	virtual void onResize() {}
	virtual void prepare() = 0;
	virtual void preProcess() {}
	virtual void render() = 0;
	virtual	void destroy() = 0;
protected:
	int _index;
	bool _isEnaled;
};

#endif // POST_PROCESSOR_H
