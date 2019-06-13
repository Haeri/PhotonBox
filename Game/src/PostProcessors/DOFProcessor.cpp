#ifndef DOF_PROCESSOR_CPP
#define DOF_PROCESSOR_CPP

#include <core/system/Renderer.h>
#include <resource/PostProcessor.h>
#include <resource/Material.h>
#include <resource/FrameBuffer.h>

#include "../Shader/DOFShader.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class DOFProcessor : public PostProcessor
{
public:
	DOFProcessor(int index) : PostProcessor(index, 1.0f, true)
	{
		_dofShader = new Material(DOFShader::getInstance());
		//_dofShader->setProperty("depth", 7.0f);
		_dofShader->setImageBuffer("renderTexture", mainBuffer->getAttachment("color"));
		_dofShader->setImageBuffer("gPosition", Renderer::getGBuffer()->getAttachment("gPosition"));
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