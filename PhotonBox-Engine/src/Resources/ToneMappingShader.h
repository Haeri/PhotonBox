#ifndef TONE_MAPPING_SHADER
#define TONE_MAPPING_SHADER

#include "Shader.h"
#include "Vertex.h"

class ToneMappingShader : public InstancedShader<ToneMappingShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/tonemapping");
	}

	void addUniforms() override
	{
		addTexture("renderTexture");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // TONE_MAPPING_SHADER