#ifndef DEFERRED_SHADER_H
#define DEFERRED_SHADER_H

#include "PhotonBox/components/Camera.h"
#include "PhotonBox/resources/Shader.h"
#include "PhotonBox/resources/Resources.h"

class DeferredShader : public InstancedShader<DeferredShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/deferre-rendering/deferredShader");
	}

	void update(Transform* transform) override
	{
		//glUniform3fv(uniforms["viewPos"], 1, &(Camera::getMainCamera()->getViewMatrix() * Vector4f(Camera::getMainCamera()->transform->getPositionWorld(), 1.0f)).x());
		glUniformMatrix4fv(uniforms["viewMatrixInv"], 1, GL_FALSE, &(Camera::getMainCamera()->getViewMatrix().inverse()(0, 0)));
	}

	void addUniforms() override
	{
		//addUniform("viewPos");
		addUniform("viewMatrixInv");

		for (size_t i = 0; i < 3; i++)
		{
			addUniform("directionalLights[" + std::to_string(i) + "].direction");
			addUniform("directionalLights[" + std::to_string(i) + "].color");
			addUniform("directionalLights[" + std::to_string(i) + "].lightSpaceMatrix");
			addUniform("directionalLights[" + std::to_string(i) + "].intensity");
		}

		addTexture("gPosition");
		addTexture("gNormal");
		addTexture("gRoughness");
		addTexture("gMetallic");
		addTexture("gAlbedo");

		addTexture("shadowMap");
	}

	void addAttributes() override
	{
		addAttribut("aPos", Vertex::AttibLocation::POSITION);
	}
};

#endif // DEFERRED_SHADER_H
