#ifndef SSAO_PROCESSOR_CPP
#define SSAO_PROCESSOR_CPP

#include <random>

#include <Core/PostProcessor.h>
#include <Math/Math.h>

#include "../Shader/SSAOBlurShader.cpp"
#include "../Shader/SSAOShader.cpp"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class SSAOProcessor : public PostProcessor
{
public:

	SSAOProcessor(int index) : PostProcessor(index)
	{
		mainBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight());
		mainBuffer->addTextureAttachment("color", true);
		mainBuffer->ready();
		ssaoBlurBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight());
		ssaoBlurBuffer->addTextureAttachment("color", true);
		ssaoBlurBuffer->ready();

		ssaoMaterial = new Material(SSAOShader::getInstance());
		ssaoMaterial->setTexture("gPosition", Renderer::getGBuffer(), "gPosition");
		ssaoMaterial->setTexture("gNormal", Renderer::getGBuffer(), "gNormal");
		ssaoBlurMaterial = new Material(SSAOBlurShader::getInstance());
		ssaoBlurMaterial->setTexture("original", mainBuffer, "color");
		ssaoBlurMaterial->setTexture("ssaoInput", ssaoBlurBuffer, "color");

		generateNoise();
	}

	void enable() override
	{
		mainBuffer->enable();
	}

	void preProcess() override
	{
		ssaoBlurBuffer->enable();

		// TODO: Clean up this block
		ssaoMaterial->shader->bind();

		glActiveTexture(GL_TEXTURE0 + SSAOShader::getInstance()->textures["texNoise"].unit);
		glBindTexture(GL_TEXTURE_2D, _noiseTexture);

		for (unsigned int i = 0; i < 64; ++i)
		{
			ssaoMaterial->shader->setUniform("samples[" + std::to_string(i) + "]", _ssaoKernel[i]);
		}
		mainBuffer->render(ssaoMaterial);
	}

	void render() override
	{
		ssaoBlurBuffer->render(ssaoBlurMaterial);
	}

	void destroy() override
	{
		delete ssaoMaterial;
		delete ssaoBlurMaterial;

		delete mainBuffer;
		delete ssaoBlurBuffer;

		glDeleteTextures(1, &_noiseTexture);
	}
private:
	Material * ssaoMaterial, *ssaoBlurMaterial;
	FrameBuffer* mainBuffer, *ssaoBlurBuffer;

	GLuint _noiseTexture;
	std::vector<Vector3f> _ssaoKernel;

	void generateNoise()
	{

		// generate sample kernel
		// ----------------------
		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		for (unsigned int i = 0; i < 64; ++i)
		{
			Vector3f sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
			sample = sample.normalize();
			sample = sample * randomFloats(generator);
			float scale = float(i) / 64.0;

			// scale samples s.t. they're more aligned to center of kernel
			scale = Math::lerp(0.1f, 1.0f, scale * scale);
			sample = sample * scale;
			_ssaoKernel.push_back(sample);
		}

		// generate noise texture
		// ----------------------
		std::vector<Vector3f> ssaoNoise;
		for (unsigned int i = 0; i < 16; i++)
		{
			Vector3f noise(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, 0.0f); // rotate around z-axis (in tangent space)
			ssaoNoise.push_back(noise);
		}

		//TODO: LEAK: The texture never gets deleted
		glGenTextures(1, &_noiseTexture);
		glBindTexture(GL_TEXTURE_2D, _noiseTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
};

#endif // SSAO_PROCESSOR_CPP