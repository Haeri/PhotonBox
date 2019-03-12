#ifndef FORWARD_POINT_LIGHT_SHADER_H
#define FORWARD_POINT_LIGHT_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"
#include "PhotonBox/component/PointLight.h"

class ForwardPointLightShader : public InstancedShader<ForwardPointLightShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/forward-rendering/point_light");
	}

	void update(Transform* transform, LightEmitter* light)
	{

		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		PointLight* pointLight = dynamic_cast<PointLight*>(light);

		setUniform("mvp", mvp);
		setUniform("modelMatrix", transform->getTransformationMatrix());
		setUniform("viewPos", Camera::getMainCamera()->transform->getPositionWorld());
		setUniform("light.position", pointLight->transform->getPositionWorld());
		setUniform("light.intensity", pointLight->intensity);
		setUniform("light.color", pointLight->color);
		setUniform("light.constant", pointLight->constant);
		setUniform("light.linear", pointLight->linear);
		setUniform("light.quadratic", pointLight->quadratic);
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}
};

#endif // FORWARD_POINT_LIGHT_SHADER_H 
