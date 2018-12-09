#ifndef DEFAULT_POST_SHADER_H
#define DEFAULT_POST_SHADER_H

#include "PhotonBox/resources/Shader.h"
#include "PhotonBox/resources/Vertex.h"
#include "PhotonBox/resources/Resources.h"

class DefaultPostShader : public InstancedShader<DefaultPostShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/util/default-post");
	}

	void addUniforms() override
	{
		addTexture("renderTexture");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}

	Type getType() override
	{
		return Shader::Type::SCREEN_SHADER;
	}
};

#endif // DEFAULT_POST_SHADER_H
