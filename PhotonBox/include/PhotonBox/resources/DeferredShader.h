#ifndef DEFERRED_SHADER_H
#define DEFERRED_SHADER_H

#include "PhotonBox/components/Camera.h"
#include "PhotonBox/resources/Shader.h"
#include "PhotonBox/resources/Resources.h"

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
		glUniformMatrix4fv(uniforms["viewMatrixInv"], 1, GL_FALSE, &(Camera::getMainCamera()->getViewMatrix().inverse()(0, 0)));
	}

	void addUniforms() override
	{
		addUniform("viewMatrixInv");
		addUniform("numPointLights");
		addUniform("numDirectionalLights");

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

		addTexture("gPosition");
		addTexture("gNormal");
		addTexture("gRoughness");
		addTexture("gMetallic");
		addTexture("gAlbedo");
		addTexture("gIrradiance");
		addTexture("gRadiance");

		addTexture("shadowMap");
	}

	void addAttributes() override
	{
		addAttribut("aPos", Vertex::AttibLocation::POSITION);
	}
};

#endif // DEFERRED_SHADER_H
