#ifndef SPECULAR_CONVOLUTION_SHADER_H
#define SPECULAR_CONVOLUTION_SHADER_H

#include "PhotonBox/resources/Shader.h"

class SpecularConvolutionShader : public InstancedShader<SpecularConvolutionShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/util/specular-convolution");
	}

	void addUniforms() override
	{
		addUniform("projection");
		addUniform("view");
		addUniform("roughness");

		addTexture("environmentMap");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // SPECULAR_CONVOLUTION_SHADER_H
