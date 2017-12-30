#ifndef SSREFLECTION_PROCESSOR_H
#define SSREFLECTION_PROCESSOR_H

#include <random>
#include "../Core/PostProcessor.h"
#include "../Math/Math.h"
#include "../Resources/SSAOShader.h"
#include "../Resources/SSAOBlurShader.h"
#include "../Core/DeferredBuffer.h"
#include "../Core/FrameBuffer.h"
#include "../Core/Systems/Renderer.h"
#include "../Resources/Material.h"
#include "../Resources/SSReflectionShader.h"

class SSReflectionProcessor : public PostProcessor {
public:
	SSReflectionProcessor(int index) : PostProcessor(index) {
		mainBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight());
		mainBuffer->addTextureAttachment("color", true);
		mainBuffer->ready();

		ssreflection = new Material(SSReflectionShader::getInstance());
		ssreflection->setTexture("gFinalImage", mainBuffer, "color");
		ssreflection->setTexture("gPosition", Renderer::defBuffer.gBuffer, "gPosition");
		ssreflection->setTexture("gNormal", Renderer::defBuffer.gBuffer, "gNormal");
		ssreflection->setTexture("gExtraComponents", Renderer::defBuffer.gBuffer, "gMetallic");
		ssreflection->setTexture("ColorBuffer", Renderer::defBuffer.gBuffer, "gRoughness");
	}

	void enable() override {
		mainBuffer->enable();
	}

	void render() override {
		mainBuffer->render("color");
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);
		mainBuffer->render(ssreflection);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}

	void destroy() override {
		delete ssreflection;

		delete mainBuffer;
	}

private:
	Material *ssreflection;
	FrameBuffer* mainBuffer;
};

#endif // SSREFLECTION_PROCESSOR_H