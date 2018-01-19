#ifndef SSAO_BLUR_SHADER_H
#define SSAO_BLUR_SHADER_H

#include "Shader.h"
#include "Vertex.h"

class SSAOBlurShader : public InstancedShader<SSAOBlurShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/ssaoBlur");
	}

	void update(Transform* transform) override
	{
		glUniform1f(uniforms["screenWidth"], GLfloat(Display::getWidth()));
		glUniform1f(uniforms["screenHeight"], GLfloat(Display::getHeight()));
	}

	void addUniforms() override
	{
		addTexture("original");
		addTexture("ssaoInput");
		addUniform("screenWidth");
		addUniform("screenHeight");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // SSAO_BLUR_SHADER_H