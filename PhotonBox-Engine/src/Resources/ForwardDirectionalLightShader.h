#ifndef FORWARD_DIRECTIONAL_LIGHT_SHADER_H
#define FORWARD_DIRECTIONAL_LIGHT_SHADER_H

#include "Shader.h"
#include "../Components/DirectionalLight.h"
#include "../Components/Transform.h"
#include "../Components/Camera.h"

class ForwardDirectionalLightShader : public InstancedShader<ForwardDirectionalLightShader> {
public:
	std::string getFilePath() override {
		return std::string("./res/PBS/directional_light");
	}

	//void update(Matrix4f& mvp, Matrix4f& modelMatrix, DirectionalLight& directionalLight, Vector4f& eyeTransformed) {
	
	void update(Transform* transform, LightEmitter* light) {

		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		Vector4f eyePos = Vector4f(Camera::getMainCamera()->transform->getPositionWorld(), 1);
		DirectionalLight* dl = dynamic_cast<DirectionalLight*>(light);
		Vector3f lvp = dl->direction;

		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
		glUniformMatrix4fv(uniforms["modelMatrix"], 1, GL_FALSE, &(transform->getTransformationMatrix()(0, 0)));
		glUniform3fv(uniforms["viewPos"], 1, &(eyePos.x()));
		glUniform3fv(uniforms["light.direction"], 1, &(lvp.x()));
		glUniform3fv(uniforms["light.color"], 1, &(dl->color.x()));
		glUniform1f(uniforms["light.intensity"], dl->intensity);
	}

	void addUniforms() override {
		addUniform("mvp");
		addUniform("modelMatrix");
		addUniform("viewPos");
		addUniform("light.direction");
		addUniform("light.color");
		addUniform("light.intensity");

		addTexture("albedoMap");
		addTexture("normalMap");
		addTexture("roughnessMap");
		addTexture("metallicMap");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}
};

#endif // FORWARD_DIRECTIONAL_LIGHT_SHADER_H
