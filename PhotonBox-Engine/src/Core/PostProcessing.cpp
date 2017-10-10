#include "PostProcessing.h"
#include "../Resources/PostShader.h"
#include "PostProcessor.h"

std::map<int, PostProcessor*> PostProcessing::_processorMap;
bool PostProcessing::_doPostProcessing;

void PostProcessing::addProcessor(PostProcessor * processor){
	_processorMap[processor->getIndex()] = processor;
}

void PostProcessing::removeProcessor(PostProcessor* processor){
	_processorMap.erase(processor->getIndex());
}

void PostProcessing::init() {
	for (std::map<int, PostProcessor*>::const_iterator it = _processorMap.begin(); it != _processorMap.end(); ++it){
		it->second->init();
	}
}

void PostProcessing::preProcess() {
	if (_processorMap.begin() == _processorMap.end()) {
		_doPostProcessing = false;
		return;
	}
	_processorMap.begin()->second->enable();
}

void PostProcessing::postProcess() {
	if (!_doPostProcessing) return;

	for (std::map<int, PostProcessor*>::const_iterator it = _processorMap.begin(); it != (--_processorMap.end()); ++it) {
		(++it)->second->enable();
		(--it)->second->render();
	}

	FrameBuffer::resetDefaultBuffer();
	glDisable(GL_DEPTH_TEST);
	(--_processorMap.end())->second->render();
}

void PostProcessing::destroy() {
	for (std::map<int, PostProcessor*>::const_iterator it = _processorMap.begin(); it != _processorMap.end(); ++it) {
		it->second->destroy();
	}
}