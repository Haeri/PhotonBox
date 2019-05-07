#ifndef DOF_PROCESSOR_CPP
#define DOF_PROCESSOR_CPP

#include <resource/PostProcessor.h>
#include <resource/Material.h>
#include <core/system/Renderer.h>

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
		_dofShader->setTexture("renderTexture", mainBuffer, "color");
		_dofShader->setTexture("gPosition", Renderer::getGBuffer(), "gPosition");
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