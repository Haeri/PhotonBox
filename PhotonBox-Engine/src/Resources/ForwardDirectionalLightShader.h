#ifndef FORWARD_DIRECTIONAL_LIGHT_SHADER_H
#define FORWARD_DIRECTIONAL_LIGHT_SHADER_H


#include "Shader.h"
#include "../Components/DirectionalLight.h"
#include "../Components/Transform.h"
#include "../Components/Camera.h"
#include "../Resources/Vertex.h"

class ForwardDirectionalLightShader : public InstancedShader<ForwardDirectionalLightShader> {
public:
	std::string getFilePath() override {
		return std::string("./res/shaders/forward-rendering/directional_light");
	}

	void update(Transform* transform, LightEmitter* light) {
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		Vector4f eyePos = Vector4f(Camera::getMainCamera()->transform->getPositionWorld(), 1);
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

		glUniformMatrix4fv(uniforms["lightSpaceMatrix"], 1, GL_FALSE, &(lightSpaceMatrix(0, 0)));
		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
		glUniformMatrix4fv(uniforms["modelMatrix"], 1, GL_FALSE, &(transform->getTransformationMatrix()(0, 0)));
		glUniform3fv(uniforms["viewPos"], 1, &(eyePos.x()));
		glUniform3fv(uniforms["light.direction"], 1, &(lvp.x()));
		glUniform3fv(uniforms["light.color"], 1, &(dl->color.x()));
		glUniform1f(uniforms["light.intensity"], dl->intensity);

		glActiveTexture(textures["shadowMap"].unit);
		glBindTexture(GL_TEXTURE_2D, dl->_depthMap);
	}

	void addUniforms() override {
		addUniform("mvp");
		addUniform("modelMatrix");
		addUniform("viewPos");
		addUniform("lightSpaceMatrix");
		addUniform("light.direction");
		addUniform("light.color");
		addUniform("light.intensity");

		addTexture("albedoMap");
		addTexture("normalMap");
		addTexture("roughnessMap");
		addTexture("metallicMap");
		addTexture("shadowMap");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}
};

#endif // FORWARD_DIRECTIONAL_LIGHT_SHADER_H
