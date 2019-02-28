#ifndef DEFERRED_SHADER_H
#define DEFERRED_SHADER_H

#include "PhotonBox/components/Camera.h"
#include "PhotonBox/resources/Shader.h"
#include "PhotonBox/resources/Resources.h"
#include "PhotonBox/core/Time.h"


class DeferredShader : public InstancedShader<DeferredShader>
{
public:
	const unsigned int MAX_DIRECTIONAL_LIGHTS = 3;
	const unsigned int MAX_POINT_LIGHTS = 10;
	const unsigned int MAX_SPOT_LIGHTS = 10;

	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/deferre-rendering/deferredShader");
	}

	void update(Transform* transform) override
	{
		glUniformMatrix4fv(uniforms["projection"], 1, GL_FALSE, &(Camera::getMainCamera()->getProjectionMatrix()(0, 0)));
		glUniformMatrix4fv(uniforms["viewMatrixInv"], 1, GL_FALSE, &(Camera::getMainCamera()->getViewMatrix().inverse()(0, 0)));
	}

	void addUniforms() override
	{
		addUniform("projection");
		addUniform("viewMatrixInv");

		addUniform("numDirectionalLights");
		addUniform("numPointLights");
		addUniform("numSpotLights");

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
		addTexture("gNormal");
		addTexture("gRoughness");
		addTexture("gMetallic");
		addTexture("gAlbedo");
		addTexture("gIrradiance");
		addTexture("gRadiance");
		addTexture("gEmission");

		addTexture("shadowMap");
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

#endif // DEFERRED_SHADER_H
