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
		glUniformMatrix4fv(uniforms["viewMatrixInv"], 1, GL_FALSE, &(Camera::getMainCamera()->getViewMatrix().inverse()(0, 0)));
		glUniformMatrix4fv(uniforms["projectionMatrixInv"], 1, GL_FALSE, &(Camera::getMainCamera()->getProjectionMatrix().inverse()(0, 0)));

		//glUniform1f(uniforms["time"], static_cast<float>(Time::time));
	}

	void addUniforms() override
	{
		addUniform("viewMatrixInv");
		addUniform("projectionMatrixInv");
		addUniform("numDirectionalLights");
		addUniform("numPointLights");
		addUniform("numSpotLights");

		addUniform("time");

		for (size_t i = 0; i < MAX_DIRECTIONAL_LIGHTS; ++i)
		{
			addUniform("directionalLights[" + std::to_string(i) + "].direction");
			addUniform("directionalLights[" + std::to_string(i) + "].color");
			addUniform("directionalLights[" + std::to_string(i) + "].lightSpaceMatrix");
			addUniform("directionalLights[" + std::to_string(i) + "].intensity");
		}

		for (size_t i = 0; i < MAX_POINT_LIGHTS; ++i)
		{
			addUniform("pointLights[" + std::to_string(i) + "].position");
			addUniform("pointLights[" + std::to_string(i) + "].color");
			addUniform("pointLights[" + std::to_string(i) + "].intensity");
			addUniform("pointLights[" + std::to_string(i) + "].attenuation.constant");
			addUniform("pointLights[" + std::to_string(i) + "].attenuation.linear");
			addUniform("pointLights[" + std::to_string(i) + "].attenuation.quadratic");
		}

		for (size_t i = 0; i < MAX_SPOT_LIGHTS; ++i)
		{
			addUniform("spotLights[" + std::to_string(i) + "].position");
			addUniform("spotLights[" + std::to_string(i) + "].direction");
			addUniform("spotLights[" + std::to_string(i) + "].coneAngle");
			addUniform("spotLights[" + std::to_string(i) + "].coneFallOff");
			addUniform("spotLights[" + std::to_string(i) + "].color");
			addUniform("spotLights[" + std::to_string(i) + "].intensity");
			addUniform("spotLights[" + std::to_string(i) + "].attenuation.constant");
			addUniform("spotLights[" + std::to_string(i) + "].attenuation.linear");
			addUniform("spotLights[" + std::to_string(i) + "].attenuation.quadratic");
		}

		addTexture("gPosition");

		addTexture("shadowMap");
		addTexture("noise");
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
