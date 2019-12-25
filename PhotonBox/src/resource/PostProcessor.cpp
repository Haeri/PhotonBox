#include "PhotonBox/resource/PostProcessor.h"

#include "PhotonBox/core/system/PostProcessing.h"
#include "PhotonBox/resource/FrameBuffer.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

PostProcessor::PostProcessor(int index, float scale, bool mipmap) : _index(index)
{
	mainBuffer = new FrameBuffer(scale);
	mainBuffer->addTextureAttachment("color", true, mipmap);
	mainBuffer->ready();

	setEnabled(true);
}

PostProcessor::~PostProcessor()
{
	delete mainBuffer;
}

int PostProcessor::getIndex()
{
	return _index; 
}

bool PostProcessor::isEnabled()
{
	return _isEnaled; 
}

void PostProcessor::setEnabled(bool enabled)
{
	_isEnaled = enabled;
	if (enabled)
	{
		PostProcessing::addProcessor(this);
	}
	else
	{
		PostProcessing::removeProcessor(this);
	}

	onEnableChange(enabled);
}