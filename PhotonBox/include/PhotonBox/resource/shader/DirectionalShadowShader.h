#ifndef DIRECTIONAL_SHADOW_SHADER_H
#define DIRECTIONAL_SHADOW_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/component/DirectionalLight.h"
#include "PhotonBox/component/Transform.h"
#include "PhotonBox/core/Resources.h"

class DirectionalShadowShader : public InstancedShader<DirectionalShadowShader>
{
public:
	float offset = -3;

	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/util/directionalShadow");
	}

	void update(Transform* transform, LightEmitter* light)
	{
		Matrix4f model = transform->getTransformationMatrix();
		DirectionalLight* dl = static_cast<DirectionalLight*>(light);
		/*
		Matrix4f lightView = Matrix4f::lookAt(
			Camera::getMainCamera()->transform->getPosition() + (dl->direction * -3),
			Vector3f(0.0f, 1.0f, 0.0f),
			dl->direction);*/
		Matrix4f lightView = Matrix4f::lookAt(
			(dl->direction * offset),
			Vector3f(0.0f, 1.0f, 0.0f),
			dl->direction);
		Matrix4f lightSpaceMatrix = dl->lightProjection * lightView;

		glUniformMatrix4fv(uniforms["lightSpaceMatrix"], 1, GL_FALSE, &(lightSpaceMatrix(0, 0)));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, &(model(0, 0)));
	}

	void addUniforms() override
	{
		addUniform("lightSpaceMatrix");
		addUniform("model");

		addTexture("albedoMap");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // DIRECTIONAL_SHADOW_SHADER_H
