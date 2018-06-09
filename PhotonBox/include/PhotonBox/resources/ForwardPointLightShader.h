#ifndef FORWARD_POINT_LIGHT_SHADER_H
#define FORWARD_POINT_LIGHT_SHADER_H

#include "PhotonBox/components/Camera.h"
#include "PhotonBox/components/PointLight.h"
#include "PhotonBox/components/Transform.h"
#include "PhotonBox/resources/Shader.h"
#include "PhotonBox/resources/Resources.h"

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
		Vector4f eyePos = Vector4f(Camera::getMainCamera()->transform->getPositionWorld(), 1);
		PointLight* pointLight = dynamic_cast<PointLight*>(light);
		Vector3f lightPos = pointLight->transform->getPositionWorld();

		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
		glUniformMatrix4fv(uniforms["modelMatrix"], 1, GL_FALSE, &(transform->getTransformationMatrix()(0, 0)));
		glUniform3fv(uniforms["viewPos"], 1, &(eyePos.x()));
		glUniform3fv(uniforms["light.position"], 1, &(lightPos.x()));
		glUniform3fv(uniforms["light.color"], 1, &(pointLight->color.x()));
		glUniform1f(uniforms["light.intensity"], pointLight->intensity);
		glUniform1f(uniforms["light.constant"], pointLight->constant);
		glUniform1f(uniforms["light.linear"], pointLight->linear);
		glUniform1f(uniforms["light.quadratic"], pointLight->quadratic);
	}

	void addUniforms()
	{
		addUniform("mvp");
		addUniform("modelMatrix");
		addUniform("viewPos");
		addUniform("light.position");
		addUniform("light.color");
		addUniform("light.intensity");
		addUniform("light.constant");
		addUniform("light.linear");
		addUniform("light.quadratic");

		addTexture("albedoMap");
		addTexture("normalMap");
		addTexture("roughnessMap");
		addTexture("metallicMap");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}
};

#endif /* defined(FORWARD_POINT_LIGHT_SHADER_H) */
