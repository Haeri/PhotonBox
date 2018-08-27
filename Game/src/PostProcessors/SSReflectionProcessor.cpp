#ifndef SSREFLECTION_PROCESSOR_CPP
#define SSREFLECTION_PROCESSOR_CPP

#include <random>

#include <Core/FrameBuffer.h>
#include <Core/PostProcessor.h>
#include <Core/Systems/Renderer.h>
#include <Math/Math.h>
#include <Resources/Material.h>

#include "../Shader/SSAOBlurShader.cpp"
#include "../Shader/SSAOShader.cpp"
#include "../Shader/SSReflectionShader.cpp"

class SSReflectionProcessor : public PostProcessor
{
public:
	SSReflectionProcessor(int index) : PostProcessor(index)
	{
		_mainBuffer = new FrameBuffer(1);
		_mainBuffer->addTextureAttachment("color", true);
		_mainBuffer->ready();

		_ssreflection = new Material(SSReflectionShader::getInstance());
		_ssreflection->setTexture("mainBuffer", _mainBuffer, "color");
		_ssreflection->setTexture("gPosition", Renderer::getGBuffer(), "gPosition");
		_ssreflection->setTexture("gNormal", Renderer::getGBuffer(), "gNormal");
		_ssreflection->setTexture("gMetallic", Renderer::getGBuffer(), "gMetallic");
		_ssreflection->setTexture("gRoughness", Renderer::getGBuffer(), "gRoughness");
	}

	void enable() override
	{
		_mainBuffer->enable();
	}

	void render() override
	{
		_mainBuffer->render("color");
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);
		_mainBuffer->render(_ssreflection);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}

	void destroy() override
	{
		delete _ssreflection;
		delete _mainBuffer;
	}

private:
	Material * _ssreflection;
	FrameBuffer* _mainBuffer;
};

#endif // SSREFLECTION_PROCESSOR_CPP