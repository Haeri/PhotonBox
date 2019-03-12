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
		Vector4f eyePos = Vector4f(Camera::getMainCamera()->transform->getPositionWorld(), 1);
		Matrix4f mv = Camera::getMainCamera()->getViewProjection();
		Matrix4f mvp = mv * transform->getTransformationMatrix();
		Matrix4f oldmvp = _oldModelViewMatrix * transform->getTransformationMatrix();

		/*
		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
		glUniformMatrix4fv(uniforms["mvpOld"], 1, GL_FALSE, &(oldmvp(0, 0)));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, &(transform->getTransformationMatrix()(0, 0)));
		glUniformMatrix4fv(uniforms["view"], 1, GL_FALSE, &(Camera::getMainCamera()->getViewMatrix()(0, 0)));
		glUniform3fv(uniforms["viewPos"], 1, &(eyePos.x()));
		*/

		setUniform("mvp", mvp);
		setUniform("mvpOld", oldmvp);
		setUniform("model", transform->getTransformationMatrix());
		setUniform("view", Camera::getMainCamera()->getViewMatrix());
		setUniform("viewPos", Camera::getMainCamera()->transform->getPositionWorld());

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
