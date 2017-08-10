#ifndef FORWARD_POINT_LIGHT_SHADER_H
#define FORWARD_POINT_LIGHT_SHADER_H

#include "Shader.h"
#include "PointLight.h"
#include "Transform.h"

class ForwardPointLightShader : public Shader{
public:
	ForwardPointLightShader(const std::string& fileName) : Shader(fileName) {}

	void update(Matrix4f& matrix, PointLight& pointLight) {
		glUniformMatrix4fv(uniforms["transform"], 1, GL_FALSE, &(matrix(0, 0)));
		glUniform3fv(uniforms["lightPosition"], 1, &(pointLight.transform->getPosition().x()));
		glUniform1f(uniforms["lightIntensity"], pointLight.intensity);
		glUniform4fv(uniforms["lightColor"], 1, &(pointLight.color.x()));
	}

	void addUniforms() {
		addUniform("transform");
		addUniform("lightPosition");
		addUniform("lightIntensity");
		addUniform("lightColor");
	}
};

#endif /* defined(FORWARD_POINT_LIGHT_SHADER_H) */