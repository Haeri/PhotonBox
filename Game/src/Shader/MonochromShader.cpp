#ifndef MONOCHROM_SHADER_CPP
#define MONOCHROM_SHADER_CPP

#include <Resources/Shader.h>

class MonochromShader : public InstancedShader<MonochromShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/monochrom");
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

#endif // MONOCHROM_SHADER_CPP