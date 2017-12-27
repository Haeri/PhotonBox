#ifndef SSAO_PROCESSOR_H
#define SSAO_PROCESSOR_H

#include <random>
#include "../Core/PostProcessor.h"
#include "../Math/Math.h"
#include "../Resources/SSAOShader.h"
#include "../Resources/SSAOBlurShader.h"
#include "../Core/DeferredBuffer.h"

class SSAOProcessor : public PostProcessor {
public:

	SSAOProcessor(int index) : PostProcessor(index) {
		mainBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight());
		mainBuffer->addTextureAttachment("color", false, true);
		ssaoBlurBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight());
		ssaoBlurBuffer->addTextureAttachment("color", false, true);

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

		Renderer::defBuffer.gBuffer->bind(SSAOShader::getInstance()->textures["gPosition"].unit, "gPosition");
		Renderer::defBuffer.gBuffer->bind(SSAOShader::getInstance()->textures["gNormal"].unit, "gNormal");

/*		glActiveTexture(SSAOShader::getInstance()->textures["gPosition"].unit);
		glBindTexture(GL_TEXTURE_2D, Renderer::defBuffer.gBuffer->);

		glActiveTexture(SSAOShader::getInstance()->textures["gNormal"].unit);
		glBindTexture(GL_TEXTURE_2D, Renderer::defBuffer.gNormal);
*/
		glActiveTexture(SSAOShader::getInstance()->textures["texNoise"].unit);
		glBindTexture(GL_TEXTURE_2D, _noiseTexture);

		for (unsigned int i = 0; i < 64; ++i) {
			ssaoMaterial->shader->setUniform("samples[" + std::to_string(i) + "]", _ssaoKernel[i]);
		}
		mainBuffer->render("color", ssaoMaterial);
	}

	void render() override {
		ssaoBlurBuffer->render("color", ssaoBlurMaterial);
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