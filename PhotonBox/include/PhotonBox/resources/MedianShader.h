#ifndef MEDIAN_SHADER_H
#define MEDIAN_SHADER_H

#include "PhotonBox/resources/Shader.h"
#include "PhotonBox/resources/Vertex.h"
#include "PhotonBox/resources/Resources.h"

class MedianShader : public InstancedShader<MedianShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/util/median");
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

#endif // MEDIAN_SHADER_H