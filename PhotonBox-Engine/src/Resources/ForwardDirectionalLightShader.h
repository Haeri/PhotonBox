#ifndef FORWARD_DIRECTIONAL_LIGHT_SHADER_H
#define FORWARD_DIRECTIONAL_LIGHT_SHADER_H

#include "Shader.h"
#include "../Components/DirectionalLight.h"
#include "../Components/Transform.h"
#include "../Components/Camera.h"

class ForwardDirectionalLightShader : public Shader {
public:
	float shininess = 1;

	ForwardDirectionalLightShader(const std::string& fileName) { init(fileName); }

	void update(Matrix4f& mvp, Matrix4f& modelMatrix, DirectionalLight& directionalLight, Vector4f& eyeTransformed) {
		Vector3f lvp = directionalLight.direction;
		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
		glUniformMatrix4fv(uniforms["modelMatrix"], 1, GL_FALSE, &(modelMatrix(0, 0)));
		glUniform3fv(uniforms["viewPos"], 1, &(eyeTransformed.x()));
		glUniform3fv(uniforms["light.direction"], 1, &(lvp.x()));
		glUniform3fv(uniforms["light.color"], 1, &(directionalLight.color.x()));
		glUniform1f(uniforms["light.intensity"], directionalLight.intensity);
		glUniform1f(uniforms["shininess"], shininess);
	}

	void addUniforms() override {
		addUniform("mvp");
		addUniform("modelMatrix");
		addUniform("light.direction");
		addUniform("light.color");
		addUniform("light.intensity");
		addUniform("viewPos");
		addUniform("shininess");

		addUniform("specularMap");
		addUniform("normalMap");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}
};

#endif // FORWARD_DIRECTIONAL_LIGHT_SHADER_H
