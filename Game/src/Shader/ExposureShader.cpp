#ifndef EXPOSURE_SHADER_CPP
#define EXPOSURE_SHADER_CPP

#include <Resources/Shader.h>

class ExposureShader : public InstancedShader<ExposureShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/exposure");
	}

	void addUniforms() override
	{
		addTexture("renderTexture");
		addTexture("exposureSample");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // EXPOSURE_SHADER_CPP