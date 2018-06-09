#ifndef FORWARD_SPOT_LIGHT_SHADER_H
#define FORWARD_SPOT_LIGHT_SHADER_H

#include "PhotonBox/components/Camera.h"
#include "PhotonBox/components/SpotLight.h"
#include "PhotonBox/components/Transform.h"
#include "PhotonBox/resources/Shader.h"
#include "PhotonBox/resources/Resources.h"

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
		Vector4f eyePos = Vector4f(Camera::getMainCamera()->transform->getPositionWorld(), 1);
		SpotLight* spotLight = dynamic_cast<SpotLight*>(light);
		Vector3f lightPos = spotLight->transform->getPositionWorld();
		Vector3f direction = spotLight->transform->forwardWorld();

		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
		glUniformMatrix4fv(uniforms["modelMatrix"], 1, GL_FALSE, &(transform->getTransformationMatrix()(0, 0)));
		glUniform3fv(uniforms["viewPos"], 1, &(eyePos.x()));
		glUniform3fv(uniforms["light.position"], 1, &(lightPos.x()));
		glUniform3fv(uniforms["light.direction"], 1, &(direction.x()));
		glUniform3fv(uniforms["light.color"], 1, &(spotLight->color.x()));
		glUniform1f(uniforms["light.coneAngle"], spotLight->coneAngle);
		glUniform1f(uniforms["light.coneFallOff"], spotLight->coneAttenuation);
		glUniform1f(uniforms["light.intensity"], spotLight->intensity);
		glUniform1f(uniforms["light.constant"], spotLight->constant);
		glUniform1f(uniforms["light.linear"], spotLight->linear);
		glUniform1f(uniforms["light.quadratic"], spotLight->quadratic);
	}

	void addUniforms()
	{
		addUniform("mvp");
		addUniform("modelMatrix");
		addUniform("viewPos");
		addUniform("light.position");
		addUniform("light.direction");
		addUniform("light.coneAngle");
		addUniform("light.coneFallOff");
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

#endif // FORWARD_SPOT_LIGHT_SHADER_H
