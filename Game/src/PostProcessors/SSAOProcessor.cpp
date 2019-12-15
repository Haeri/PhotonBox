#ifndef SSAO_PROCESSOR_CPP
#define SSAO_PROCESSOR_CPP

#include <random>

#include <math/Math.h>
#include <core/system/Renderer.h>
#include <resource/PostProcessor.h>
#include <resource/Material.h>
#include <resource/FrameBuffer.h>

#include "../Shader/SSAOBlurShader.cpp"
#include "../Shader/SSAOShader.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class SSAOProcessor : public PostProcessor
{
public:

	SSAOProcessor(int index) : PostProcessor(index)
	{
		ssaoBlurBuffer = new FrameBuffer(0.5f);
		ssaoBlurBuffer->addTextureAttachment("color", true);
		ssaoBlurBuffer->ready();

		ssaoMaterial = new Material(SSAOShader::getInstance());
		ssaoMaterial->setImageBuffer("gPosition", Renderer::getGBuffer()->getAttachment("gPosition"));
		ssaoMaterial->setImageBuffer("gNormal", Renderer::getGBuffer()->getAttachment("gNormal"));
		
		ssaoBlurMaterial = new Material(SSAOBlurShader::getInstance());
		ssaoBlurMaterial->setImageBuffer("original", mainBuffer->getAttachment("color"));
		ssaoBlurMaterial->setImageBuffer("ssaoInput", ssaoBlurBuffer->getAttachment("color"));
		ssaoBlurMaterial->setProperty<float>("screenWidth", Display::getWidth() / 2.0f);
		ssaoBlurMaterial->setProperty<float>("screenHeight", Display::getHeight() / 2.0f);

		generateNoise();
	}

	void onResize() override
	{
		ssaoBlurMaterial->setProperty<float>("screenWidth", Display::getWidth()/2.0f);
		ssaoBlurMaterial->setProperty<float>("screenHeight", Display::getHeight()/ 2.0f);
	}

	void render(FrameBuffer* nextBuffer) override
	{
		// Prapare
		ssaoBlurBuffer->enable();

		// TODO: Clean up this block
		ssaoMaterial->getShader()->bind();

		glActiveTexture(GL_TEXTURE0 + SSAOShader::getInstance()->textures["texNoise"].unit);
		glBindTexture(GL_TEXTURE_2D, _noiseTexture);

		for (unsigned int i = 0; i < 64; ++i)
		{
			ssaoMaterial->getShader()->setUniform("samples[" + std::to_string(i) + "]", _ssaoKernel[i]);
		}
		mainBuffer->render(ssaoMaterial);

		// Render
		nextBuffer->enable();
		ssaoBlurBuffer->render(ssaoBlurMaterial);
	}

	void destroy() override
	{
		delete ssaoMaterial;
		delete ssaoBlurMaterial;

		delete ssaoBlurBuffer;

		glDeleteTextures(1, &_noiseTexture);
	}
private:
	Material * ssaoMaterial, *ssaoBlurMaterial;
	FrameBuffer* ssaoBlurBuffer;

	GLuint _noiseTexture;
	std::vector<Vector3f> _ssaoKernel;

	void generateNoise()
	{
		// generate sample kernel
		// ----------------------
		std::uniform_real_distribution<GLfloat> randomFloats(0.0f, 1.0f); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		for (unsigned int i = 0; i < 64; ++i)
		{
			Vector3f sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
			sample = sample.normalize();
			sample = sample * randomFloats(generator);
			float scale = float(i) / 64.0f;

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