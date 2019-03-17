#ifndef FORWARD_DIRECTIONAL_LIGHT_SHADER_H
#define FORWARD_DIRECTIONAL_LIGHT_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"
#include "PhotonBox/component/DirectionalLight.h"

class ForwardDirectionalLightShader : public InstancedShader<ForwardDirectionalLightShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/forward-rendering/directional_light");
	}

	void update(Transform* transform, LightEmitter* light)
	{
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		
		DirectionalLight* dl = dynamic_cast<DirectionalLight*>(light);
		Vector3f lvp = dl->direction;

		/*Matrix4f lightView = Matrix4f::lookAt(
		Camera::getMainCamera()->transform->getPosition() + (dl->direction * -3),
		Vector3f(0.0f, 1.0f, 0.0f),
		dl->direction);*/
		Matrix4f lightView = Matrix4f::lookAt(
			(dl->direction * -3),
			Vector3f(0.0f, 1.0f, 0.0f),
			dl->direction);
		Matrix4f lightSpaceMatrix = dl->lightProjection * lightView;

		setUniform("lightSpaceMatrix", lightSpaceMatrix);
		setUniform("mvp", mvp);
		setUniform("modelMatrix", transform->getTransformationMatrix());
		setUniform("viewPos", Camera::getMainCamera()->getTransform()->getPositionWorld());
		setUniform("light.direction", lvp);
		setUniform("light.color", dl->color);
		setUniform("light.intensity", dl->intensity);
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}
};

#endif // FORWARD_DIRECTIONAL_LIGHT_SHADER_H
