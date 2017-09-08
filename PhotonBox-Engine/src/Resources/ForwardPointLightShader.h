#ifndef FORWARD_POINT_LIGHT_SHADER_H
#define FORWARD_POINT_LIGHT_SHADER_H

#include "Shader.h"
#include "../Components/PointLight.h"
#include "../Components/Transform.h"
#include "../Components/Camera.h"

class ForwardPointLightShader : public Shader{
public:
	ForwardPointLightShader(const std::string& fileName) { init(fileName); }

	void update(Matrix4f& matrix, PointLight& pointLight, Vector4f& eyeTransformed) {
		Vector3f light = pointLight.transform->getPositionWorld();
		Vector4f lvp = Camera::getMainCamera()->getViewProjection() * Vector4f(light, 1);
		glUniformMatrix4fv(uniforms["transform"], 1, GL_FALSE, &(matrix(0, 0)));
		glUniform3fv(uniforms["light.position"], 1, &(lvp.x()));
		glUniform3fv(uniforms["light.color"], 1, &(pointLight.color.x()));
		glUniform1f(uniforms["light.constant"], pointLight.constant);
		glUniform1f(uniforms["light.linear"], pointLight.linear);
		glUniform1f(uniforms["light.quadratic"], pointLight.quadratic);
		glUniform3fv(uniforms["viewPos"], 1, &(eyeTransformed.x()));
	}

	void addUniforms() {
		addUniform("transform");
		addUniform("light.position");
		addUniform("light.color");
		addUniform("light.constant");
		addUniform("light.linear");
		addUniform("light.quadratic");
		addUniform("viewPos");
	}

	void addAttributes() override {
		addAttribut("position");
		addAttribut("normal");
		addAttribut("color");
		addAttribut("uv");
	}
};

#endif /* defined(FORWARD_POINT_LIGHT_SHADER_H) */
