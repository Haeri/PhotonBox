#include "PhotonBox/core/systems/PostProcessing.h"

#include "PhotonBox/core/PostProcessor.h"
#include "PhotonBox/core/systems/Renderer.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

std::map<int, PostProcessor*> PostProcessing::_processorMap;
bool PostProcessing::_doPostProcessing;

void PostProcessing::addProcessor(PostProcessor * processor)
{
	_processorMap[processor->getIndex()] = processor;
}

void PostProcessing::removeProcessor(PostProcessor* processor)
{
	_processorMap.erase(processor->getIndex());
}

void PostProcessing::start()
{
	_doPostProcessing = (_processorMap.size() > 0);
}

void PostProcessing::postProcess()
{
	if (!_doPostProcessing) return;

	glDisable(GL_DEPTH_TEST);
	_processorMap.begin()->second->enable();
	Renderer::getMainFrameBuffer()->render("color");


	for (std::map<int, PostProcessor*>::const_iterator it = _processorMap.begin(); it != (--_processorMap.end()); ++it)
	{
		it->second->preProcess();
		(++it)->second->enable();
		(--it)->second->render();
	}

	(--_processorMap.end())->second->preProcess();
	FrameBuffer::resetDefaultBuffer();
	(--_processorMap.end())->second->render();
}

void PostProcessing::reset()
{
	for (std::map<int, PostProcessor*>::const_iterator it = _processorMap.begin(); it != _processorMap.end(); ++it)
	{
		it->second->destroy();
		delete it->second;
	}

	_processorMap.clear();
}
