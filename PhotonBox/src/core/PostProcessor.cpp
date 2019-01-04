#include "PhotonBox/core/PostProcessor.h"

PostProcessor::PostProcessor(int index) : _index(index)
{
	setEnabled(true);
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
}