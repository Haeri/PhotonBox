#ifndef G_SHADER_H
#define G_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"

class GShader : public InstancedShader<GShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/deferre-rendering/gShader");
	}

	void update(Transform* transform)
	{
		Matrix4f mv = Camera::getMainCamera()->getViewProjection();
		Matrix4f mvp = mv * transform->getTransformationMatrix();
		Matrix4f oldmvp = _oldModelViewMatrix * transform->getTransformationMatrix();

		setUniform("mvp", mvp);
		setUniform("mvpOld", oldmvp);
		setUniform("model", transform->getTransformationMatrix());
		setUniform("view", Camera::getMainCamera()->getViewMatrix());
		setUniform("viewPos", Camera::getMainCamera()->getTransform()->getPositionWorld());

		_oldModelViewMatrix = mv;
	}

	void addAttributes() override
	{
		addAttribut("aPos", Vertex::AttibLocation::POSITION);
		addAttribut("aNormal", Vertex::AttibLocation::NORMAL);
		addAttribut("aTexCoords", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}

	Type getType() override
	{
		return Shader::Type::SURFACE_SHADER;
	}

private:
	Matrix4f _oldModelViewMatrix;
};

#endif // G_SHADER_H
