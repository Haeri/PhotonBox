#ifndef FORWARD_POINT_LIGHT_SHADER_H
#define FORWARD_POINT_LIGHT_SHADER_H

#include "Shader.h"
#include "../Components/PointLight.h"
#include "../Components/Transform.h"
#include "../Components/Camera.h"

class ForwardPointLightShader : public Shader{
public:
	float shininess = 1;

	ForwardPointLightShader(const std::string& fileName) { init(fileName); }

	void update(Matrix4f& mvp, Matrix4f& modelMatrix, PointLight& pointLight, Vector4f& eyeTransformed) {
		Vector3f light = pointLight.transform->getPositionWorld();
		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
		glUniformMatrix4fv(uniforms["modelMatrix"], 1, GL_FALSE, &(modelMatrix(0, 0)));
		glUniform3fv(uniforms["viewPos"], 1, &(eyeTransformed.x()));
		glUniform3fv(uniforms["light.position"], 1, &(light.x()));
		glUniform3fv(uniforms["light.color"], 1, &(pointLight.color.x()));
		glUniform1f(uniforms["light.intensity"], pointLight.intensity);
		glUniform1f(uniforms["light.constant"], pointLight.constant);
		glUniform1f(uniforms["light.linear"], pointLight.linear);
		glUniform1f(uniforms["light.quadratic"], pointLight.quadratic);
		glUniform1f(uniforms["shininess"], shininess);
	}

	void addUniforms() {
		addUniform("mvp");
		addUniform("modelMatrix");
		addUniform("light.position");
		addUniform("light.color");
		addUniform("light.intensity");
		addUniform("light.constant");
		addUniform("light.linear");
		addUniform("light.quadratic");
		addUniform("viewPos");
		addUniform("shininess");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
	}
};

#endif /* defined(FORWARD_POINT_LIGHT_SHADER_H) */
