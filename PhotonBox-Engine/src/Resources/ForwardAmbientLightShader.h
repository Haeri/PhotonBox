#ifndef FORWARD_AMBIENT_LIGHT_SHADER_H
#define FORWARD_AMBIENT_LIGHT_SHADER_H

#include "Shader.h"
#include "../Components/LightEmitter.h"

class ForwardAmbientLightShader : public Shader {
public:
	ForwardAmbientLightShader(const std::string& fileName) : Shader(fileName) {}

	void update(Matrix4f& matrix) override{
		Vector3f color(1, 0, 0);
		glUniformMatrix4fv(uniforms["transform"], 1, GL_FALSE, &(matrix(0, 0)));
		glUniform1f(uniforms["lightIntensity"], 1);
		glUniform4fv(uniforms["lightColor"], 1, &(color.x()));
	}

	/*
	void update(Matrix4f& matrix, LightEmitter& lightEmitter) {
		glUniformMatrix4fv(uniforms["transform"], 1, GL_FALSE, &(matrix(0, 0)));
		glUniform1f(uniforms["lightIntensity"], lightEmitter.intensity);
		glUniform4fv(uniforms["lightColor"], 1, &(lightEmitter.color.x()));
	}
	*/

	void addUniforms() override{
		addUniform("transform");
		addUniform("lightIntensity");
		addUniform("lightColor");
	}
};

#endif /* defined(FORWARD_AMBIENT_LIGHT_SHADER_H) */
