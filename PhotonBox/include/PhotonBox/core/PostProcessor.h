#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

class Material;
class FrameBuffer;

#include "PhotonBox/core/systems/PostProcessing.h"

class PostProcessor
{
public:
	FrameBuffer* mainBuffer;

	PostProcessor(int index, float scale = 1.0f, bool mipmap = false);
	~PostProcessor();

	int getIndex();
	bool isEnabled();
	void setEnabled(bool enable);

	virtual void onResize() {}
	//virtual void prepare() = 0;
	//virtual void preProcess() {}
	virtual void render(FrameBuffer* nextBuffer) = 0;
	virtual	void destroy() = 0;
protected:
	int _index;
	bool _isEnaled;
};

#endif // POST_PROCESSOR_H
