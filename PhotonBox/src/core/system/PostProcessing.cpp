#include "PhotonBox/core/system/PostProcessing.h"

#include "PhotonBox/resource/PostProcessor.h"
#include "PhotonBox/core/system/Renderer.h"
#include "PhotonBox/resource/FrameBuffer.h"
#include "PhotonBox/core/system/DebugGUI.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

std::map<int, PostProcessor*> PostProcessing::_processorMap;
std::set<PostProcessor*> PostProcessing::_processorResourceSet;

void PostProcessing::addProcessor(PostProcessor * processor)
{
	if (_processorMap[processor->getIndex()] != nullptr)
		std::cerr << "Post Processor Index already occupied. Overriding\n";

	_processorMap[processor->getIndex()] = processor;

	_processorResourceSet.insert(processor);
}

void PostProcessing::removeProcessor(PostProcessor* processor)
{
	_processorMap.erase(processor->getIndex());
}

bool PostProcessing::isActive()
{
	return shouldPostProcess();
}

void PostProcessing::start() 
{

}

bool PostProcessing::shouldPostProcess()
{
	return (_processorMap.size() > 0);
}

void PostProcessing::postProcess()
{
	if (!shouldPostProcess()) return;

	glDisable(GL_DEPTH_TEST);
	_processorMap.begin()->second->mainBuffer->enable();
	Renderer::getMainFrameBuffer()->render("color");

	FrameBuffer* tmp;
	for (std::map<int, PostProcessor*>::const_iterator it = _processorMap.begin(); it != (--_processorMap.end()); ++it)
	{
		//it->second->preProcess();
		tmp = (++it)->second->mainBuffer;
		(--it)->second->render(tmp);
	}

	//(--_processorMap.end())->second->preProcess();
	//FrameBuffer::resetDefaultBuffer();
	(--_processorMap.end())->second->render(Renderer::getMainFrameBuffer());

	FrameBuffer::resetDefaultBuffer();
	Renderer::getMainFrameBuffer()->render("color");
}

void PostProcessing::resizeAll()
{
	for (std::map<int, PostProcessor*>::const_iterator it = _processorMap.begin(); it != _processorMap.end(); ++it)
	{
		it->second->onResize();
	}
}

void PostProcessing::reset()
{
	destroy();
}

void PostProcessing::destroy()
{
	for (std::set<PostProcessor*>::const_iterator it = _processorResourceSet.begin(); it != _processorResourceSet.end(); ++it)
	{
		(*it)->destroy();
		delete (*it);
	}

	_processorMap.clear();
	_processorResourceSet.clear();
}

void PostProcessing::drawGizmos()
{
	ImGui::Begin("PostProcessing");
	for (std::set<PostProcessor*>::const_iterator it = _processorResourceSet.begin(); it != _processorResourceSet.end(); ++it)
	{
		std::string name = typeid(*(*it)).name();
		size_t index = name.find("class ");
		if (index != std::string::npos) {
			name = name.replace(index, 6, "");
		}

		ImGui::BeginGroup();
		ImGui::TextUnformatted(name.c_str());
		bool toggle = (*it)->isEnabled();
		ImGui::Checkbox(("Enable##" + name).c_str(), &toggle);
			
		if (toggle != (*it)->isEnabled()) {
			(*it)->setEnabled(toggle);
		}
		ImGui::EndGroup();
		ImGui::NewLine();
	}
	ImGui::End();
}
