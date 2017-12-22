#ifndef SSAO_PROCESSOR_H
#define SSAO_PROCESSOR_H

#include "../Core/PostProcessor.h"
#include "../Game/MonochromShader.h"
#include "../Core/FrameBuffer.h"
#include "ToneMappingShader.h"
#include "AutoExposureShader.h"
#include "Material.h"
#include "Exposure.h"
#include <random>
#include "../Math/Math.h"
#include "../Resources/SSAOShader.h"
#include "../Core/Systems/Renderer.h"
#include "../Core/DeferredBuffer.h"
#include "../Resources/SSAOBlurShader.h"

class SSAOProcessor : public PostProcessor {
public:

	SSAOProcessor(int index) : PostProcessor(index) {
		mainBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight(), false);
		ssaoBlurBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight(), false);

		ssaoMaterial = new Material(SSAOShader::getInstance());
		ssaoBlurMaterial = new Material(SSAOBlurShader::getInstance());
		ssaoBlurMaterial->setTexture("original", mainBuffer);
		ssaoBlurMaterial->setTexture("ssaoInput", ssaoBlurBuffer);

		generateNoise();
	}

	void enable() override {
		mainBuffer->enable();
	}

	void preProcess() override {
		ssaoBlurBuffer->enable();
		
		ssaoMaterial->shader->bind();
		glActiveTexture(SSAOShader::getInstance()->textures["gPosition"].unit);
		glBindTexture(GL_TEXTURE_2D, Renderer::defBuffer.gPosition);

		glActiveTexture(SSAOShader::getInstance()->textures["gNormal"].unit);
		glBindTexture(GL_TEXTURE_2D, Renderer::defBuffer.gNormal);

		glActiveTexture(SSAOShader::getInstance()->textures["texNoise"].unit);
		glBindTexture(GL_TEXTURE_2D, _noiseTexture);

		for (unsigned int i = 0; i < 64; ++i) {
			//ssaoMaterial->setProperty<Vector3f>();
			ssaoMaterial->shader->setUniform("samples[" + std::to_string(i) + "]", _ssaoKernel[i]);
		}
		mainBuffer->render(ssaoMaterial);
	}

	void render() override {
		/*mainBuffer->render();

		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);*/
		ssaoBlurBuffer->render(ssaoBlurMaterial);
		/*glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);*/
	}

	void destroy() override {

	}
private:
	Material *ssaoMaterial, *ssaoBlurMaterial;
	FrameBuffer* mainBuffer, *ssaoBlurBuffer;

	GLuint _noiseTexture;
	std::vector<Vector3f> _ssaoKernel;

	void generateNoise() {
	
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
			Vector3f noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
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

#endif // SSAO_PROCESSOR_H