#ifndef FORWARD_SPOT_LIGHT_SHADER_H
#define FORWARD_SPOT_LIGHT_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"
#include "PhotonBox/component/SpotLight.h"

class ForwardSpotLightShader : public InstancedShader<ForwardSpotLightShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/forward-rendering/spot_light");
	}

	void update(Transform* transform, LightEmitter* light)
	{
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		SpotLight* spotLight = dynamic_cast<SpotLight*>(light);

		setUniform("mvp", mvp);
		setUniform("modelMatrix", transform->getTransformationMatrix());
		setUniform("viewPos", Camera::getMainCamera()->getTransform()->getPositionWorld());
		setUniform("light.position", spotLight->getTransform()->getPositionWorld());
		setUniform("light.direction", spotLight->getTransform()->forwardWorld());
		setUniform("light.intensity", spotLight->intensity);
		setUniform("light.color", spotLight->color);
		setUniform("light.coneAngle", spotLight->coneAngle);
		setUniform("light.coneFallOff", spotLight->coneAttenuation);
		setUniform("light.constant", spotLight->constant);
		setUniform("light.linear", spotLight->linear);
		setUniform("light.quadratic", spotLight->quadratic);
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}
};

#endif // FORWARD_SPOT_LIGHT_SHADER_H
