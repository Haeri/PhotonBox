#ifndef IRRADIANCE_SHADER_H
#define IRRADIANCE_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"

class IrradianceShader : public InstancedShader<IrradianceShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/util/irradiance");
	}

	void addUniforms() override
	{
		addUniform("projection");
		addUniform("view");

		addTexture("environmentMap");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // IRRADIANCE_SHADER_H
