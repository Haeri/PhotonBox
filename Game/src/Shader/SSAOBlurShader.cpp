#ifndef SSAO_BLUR_SHADER_CPP
#define SSAO_BLUR_SHADER_CPP

#include <Core/Display.h>
#include <Resources/Shader.h>

class SSAOBlurShader : public InstancedShader<SSAOBlurShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/ssaoBlur");
	}

	void addUniforms() override
	{
		addUniform("screenWidth");
		addUniform("screenHeight");

		addTexture("original");
		addTexture("ssaoInput");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // SSAO_BLUR_SHADER_CPP