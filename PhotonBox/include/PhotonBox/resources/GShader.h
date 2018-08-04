#ifndef G_SHADER_H
#define G_SHADER_H

#include "PhotonBox/resources/Shader.h"
#include "PhotonBox/resources/Vertex.h"
#include "PhotonBox/resources/Resources.h"

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

		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, &(transform->getTransformationMatrix()(0, 0)));
		glUniformMatrix4fv(uniforms["view"], 1, GL_FALSE, &(Camera::getMainCamera()->getViewMatrix()(0, 0)));
		glUniformMatrix4fv(uniforms["projection"], 1, GL_FALSE, &(Camera::getMainCamera()->getProjectionMatrix()(0, 0)));
		glUniform3fv(uniforms["viewPos"], 1, &(eyePos.x()));
	}

	void addUniforms() override
	{
		addUniform("model");
		addUniform("view");
		addUniform("projection");

		addUniform("viewPos");
		addUniform("minBound");
		addUniform("maxBound");
		addUniform("boundPos");
		addUniform("useCorrection");
		addUniform("emissionIntensity");

		addTexture("irradianceMap");
		addTexture("convolutedSpecularMap");

		addTexture("albedoMap");
		addTexture("roughnessMap");
		addTexture("metallicMap");
		addTexture("normalMap");
		addTexture("emissionMap");
		//addTexture("aoMap");
	}

	void addAttributes() override
	{
		addAttribut("aPos", Vertex::AttibLocation::POSITION);
		addAttribut("aNormal", Vertex::AttibLocation::NORMAL);
		addAttribut("aTexCoords", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}
};

#endif // G_SHADER_H
