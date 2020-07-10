#ifndef TRANSPARENT_SHADER_H
#define TRANSPARENT_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"
#include "PhotonBox/component/AmbientLight.h"

class TransparentShader : public InstancedShader<TransparentShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/forward-rendering/transparent");
	}

	void update(Transform* transform, LightEmitter* light)
	{
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		AmbientLight* al = dynamic_cast<AmbientLight*>(light);

		setUniform("mvp", mvp);
		setUniform("modelMatrix", transform->getTransformationMatrix());
		setUniform("light.intensity", al->intensity);
		setUniform("light.color", al->color);
		setUniform("viewPos", Camera::getMainCamera()->getTransform()->getPositionWorld());
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}
};

#endif // TRANSPARENT_SHADER_H
