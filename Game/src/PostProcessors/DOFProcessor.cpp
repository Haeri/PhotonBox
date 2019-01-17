#ifndef DOF_PROCESSOR_CPP
#define DOF_PROCESSOR_CPP

#include <core/PostProcessor.h>
#include <resources/Material.h>
#include <core/systems/Renderer.h>

#include "../Shader/DOFShader.cpp"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class DOFProcessor : public PostProcessor
{
public:
	DOFProcessor(int index) : PostProcessor(index)
	{
		_mainBuffer = new FrameBuffer(1);
		_mainBuffer->addTextureAttachment("color", true);
		_mainBuffer->ready();

		_dofShader = new Material(DOFShader::getInstance());
		_dofShader->setProperty("depth", 7.0f);
		_dofShader->setTexture("renderTexture", _mainBuffer, "color");
		_dofShader->setTexture("gPosition", Renderer::getGBuffer(), "gPosition");
	}

	void prepare() override
	{
		_mainBuffer->enable();
	}

	void render() override
	{
		_mainBuffer->render(_dofShader);
	}

	void destroy() override
	{
		delete _dofShader;
		delete _mainBuffer;
	}

private:
	Material * _dofShader;
	FrameBuffer* _mainBuffer;
};

#endif // DOF_PROCESSOR_CPP