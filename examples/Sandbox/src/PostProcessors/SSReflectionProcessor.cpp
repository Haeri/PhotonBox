#ifndef SSREFLECTION_PROCESSOR_CPP
#define SSREFLECTION_PROCESSOR_CPP

#include <PhotonBox/core/system/Renderer.h>
#include <PhotonBox/resource/PostProcessor.h>
#include <PhotonBox/resource/Material.h>
#include <PhotonBox/resource/FrameBuffer.h>

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
		_ssreflection->setImageBuffer("mainBuffer", mainBuffer->getAttachment("color"));
		_ssreflection->setImageBuffer("gPosition", Renderer::getGBuffer()->getAttachment("gPosition3"));
		_ssreflection->setImageBuffer("gNormal", Renderer::getGBuffer()->getAttachment("gNormal3"));
		_ssreflection->setImageBuffer("gMetallic", Renderer::getGBuffer()->getAttachment("gIrradiance3gMetallic1"), { GL_ALPHA, GL_ZERO, GL_ZERO, GL_ZERO });
		_ssreflection->setImageBuffer("gRoughness", Renderer::getGBuffer()->getAttachment("gRadiance3Roughness1"), {GL_ALPHA, GL_ZERO, GL_ZERO, GL_ZERO });
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