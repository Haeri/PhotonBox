#ifndef VOLUMETRIC_FOG_SHADER_H
#define VOLUMETRIC_FOG_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"
#include "PhotonBox/core/Time.h"

class VolumetricFogShader : public InstancedShader<VolumetricFogShader>
{
public:
	const unsigned int MAX_DIRECTIONAL_LIGHTS = 3;
	const unsigned int MAX_POINT_LIGHTS = 10;
	const unsigned int MAX_SPOT_LIGHTS = 10;

	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/deferre-rendering/volumetricFogShader");
	}

	void update(Transform* transform) override
	{
		setUniform("viewMatrixInv", Camera::getMainCamera()->getViewMatrix().inverse());
		setUniform("projectionMatrixInv", Camera::getMainCamera()->getProjectionMatrix().inverse());
	}

	void addAttributes() override
	{
		addAttribut("aPos", Vertex::AttibLocation::POSITION);
	}

	Type getType() override
	{
		return Shader::Type::CUSTOM_SHADER;
	}
};

#endif // VOLUMETRIC_FOG_SHADER_H
