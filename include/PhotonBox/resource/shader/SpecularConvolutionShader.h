#ifndef SPECULAR_CONVOLUTION_SHADER_H
#define SPECULAR_CONVOLUTION_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"

class SpecularConvolutionShader : public InstancedShader<SpecularConvolutionShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/util/specular-convolution");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}

	Type getType() override
	{
		return Shader::Type::CUSTOM_SHADER;
	}
};

#endif // SPECULAR_CONVOLUTION_SHADER_H
