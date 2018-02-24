#ifndef DEFAULT_POST_SHADER_H
#define DEFAULT_POST_SHADER_H

#include "Shader.h"
#include "Vertex.h"

class DefaultPostShader : public InstancedShader<DefaultPostShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/util/default-post");
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

#endif // DEFAULT_POST_SHADER_H
