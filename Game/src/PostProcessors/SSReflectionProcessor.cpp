#ifndef SSREFLECTION_PROCESSOR_CPP
#define SSREFLECTION_PROCESSOR_CPP

#include <core/system/Renderer.h>
#include <resource/PostProcessor.h>
#include <resource/Material.h>

#include "../Shader/SSReflectionShader.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class SSReflectionProcessor : public PostProcessor
{
public:
	SSReflectionProcessor(int index) : PostProcessor(index)
	{
		_ssreflection = new Material(SSReflectionShader::getInstance());
		_ssreflection->setTexture("mainBuffer", mainBuffer, "color");
		_ssreflection->setTexture("gPosition", Renderer::getGBuffer(), "gPosition");
		_ssreflection->setTexture("gNormal", Renderer::getGBuffer(), "gNormal");
		_ssreflection->setTexture("gMetallic", Renderer::getGBuffer(), "gMetallic");
		_ssreflection->setTexture("gRoughness", Renderer::getGBuffer(), "gRoughness");
	}

	void render(FrameBuffer* nextBuffer) override
	{
		nextBuffer->enable();
		mainBuffer->render("color");
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);
		mainBuffer->render(_ssreflection);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}

	void destroy() override
	{
		delete _ssreflection;
	}

private:
	Material * _ssreflection;
};

#endif // SSREFLECTION_PROCESSOR_CPP