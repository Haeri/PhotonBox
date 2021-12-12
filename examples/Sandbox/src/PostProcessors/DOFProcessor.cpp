#ifndef DOF_PROCESSOR_CPP
#define DOF_PROCESSOR_CPP

#include <PhotonBox/core/system/Renderer.h>
#include <PhotonBox/resource/PostProcessor.h>
#include <PhotonBox/resource/Material.h>
#include <PhotonBox/resource/FrameBuffer.h>

#include "../Shader/DOFShader.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class DOFProcessor : public PostProcessor
{
public:
	DOFProcessor(int index) : PostProcessor(index)
	{
		_dofShader = new Material(DOFShader::getInstance());
		//_dofShader->setProperty("depth", 7.0f);
		_dofShader->setImageBuffer("renderTexture", mainBuffer->getAttachment("color"));
		_dofShader->setImageBuffer("gPosition", Renderer::getGBuffer()->getAttachment("gPosition3"));
	}

	void render(FrameBuffer* nextBuffer) override
	{
		nextBuffer->enable();
		mainBuffer->render(_dofShader);
	}

	void destroy() override
	{
		delete _dofShader;
	}

private:
	Material * _dofShader;
};

#endif // DOF_PROCESSOR_CPP