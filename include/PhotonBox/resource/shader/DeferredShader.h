#ifndef DEFERRED_SHADER_H
#define DEFERRED_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"
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
		setUniform("projection", Camera::getMainCamera()->getProjectionMatrix());
		setUniform("viewMatrixInv", Camera::getMainCamera()->getViewMatrix().inverse());

		std::unordered_map<std::type_index, std::vector<LightEmitter*>> lights = Lighting::getAllLights();
		setUniform("numDirectionalLights", (int)(lights[typeid(DirectionalLight)].size()));
		setUniform("numPointLights", (int)lights[typeid(PointLight)].size());
		setUniform("numSpotLights", (int)lights[typeid(SpotLight)].size());

		for (auto const& lightvec : lights)
		{
			int i = -1;
			for (auto const& light : lightvec.second)
			{
				if (!light->getEnable() || (typeid(*(light->getLightShader())) == typeid(*(ForwardAmbientLightShader::getInstance())))) continue;

				if (typeid(*light) == typeid(DirectionalLight))
				{
					DirectionalLight* dl = dynamic_cast<DirectionalLight*>(light);
					Vector3f lightViewDirection = (Camera::getMainCamera()->getViewMatrix() * Vector4f(dl->direction, 0.0f)).xyz();

					Matrix4f lightView = Matrix4f::lookAt(
						(dl->direction * -3),
						Vector3f(0.0f, 1.0f, 0.0f),
						dl->direction);
					Matrix4f lightSpaceMatrix = dl->lightProjection * lightView;

					++i;
					setUniform("directionalLights[" + std::to_string(i) + "].lightSpaceMatrix", lightSpaceMatrix);
					setUniform("directionalLights[" + std::to_string(i) + "].direction", lightViewDirection);
					setUniform("directionalLights[" + std::to_string(i) + "].color", dl->color);
					setUniform("directionalLights[" + std::to_string(i) + "].intensity", dl->intensity);

					dl->getShadowBuffer()->bind(textures["shadowMap"].unit, "depth");
				}
				else if (typeid(*light) == typeid(PointLight))
				{
					PointLight* dl = dynamic_cast<PointLight*>(light);
					Vector3f posViewSpace = (Camera::getMainCamera()->getViewMatrix() * Vector4f(dl->getTransform()->getPositionWorld(), 1)).xyz();
					++i;
					setUniform("pointLights[" + std::to_string(i) + "].position", posViewSpace);
					setUniform("pointLights[" + std::to_string(i) + "].color", dl->color);
					setUniform("pointLights[" + std::to_string(i) + "].intensity", dl->intensity);
					setUniform("pointLights[" + std::to_string(i) + "].attenuation.constant", dl->constant);
					setUniform("pointLights[" + std::to_string(i) + "].attenuation.linear", dl->linear);
					setUniform("pointLights[" + std::to_string(i) + "].attenuation.quadratic", dl->quadratic);
				}
				else if (typeid(*light) == typeid(SpotLight))
				{
					SpotLight* dl = dynamic_cast<SpotLight*>(light);
					Vector3f posViewSpace = (Camera::getMainCamera()->getViewMatrix() * Vector4f(dl->getTransform()->getPositionWorld(), 1)).xyz();
					Vector3f directionView = (Camera::getMainCamera()->getViewMatrix() * Vector4f(dl->getTransform()->forwardWorld(), 0.0f)).xyz();

					++i;
					setUniform("spotLights[" + std::to_string(i) + "].position", posViewSpace);
					setUniform("spotLights[" + std::to_string(i) + "].direction", directionView);
					setUniform("spotLights[" + std::to_string(i) + "].coneAngle", dl->coneAngle);
					setUniform("spotLights[" + std::to_string(i) + "].coneFallOff", dl->coneAttenuation);
					setUniform("spotLights[" + std::to_string(i) + "].color", dl->color);
					setUniform("spotLights[" + std::to_string(i) + "].intensity", dl->intensity);
					setUniform("spotLights[" + std::to_string(i) + "].attenuation.constant", dl->constant);
					setUniform("spotLights[" + std::to_string(i) + "].attenuation.linear", dl->linear);
					setUniform("spotLights[" + std::to_string(i) + "].attenuation.quadratic", dl->quadratic);
				}
			}
		}
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
