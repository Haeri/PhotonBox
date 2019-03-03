#ifndef DEFAULT_POST_SHADER_H
#define DEFAULT_POST_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"

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
