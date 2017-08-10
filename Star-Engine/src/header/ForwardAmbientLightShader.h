#ifndef FORWARD_AMBIENT_LIGHT_SHADER_H
#define FORWARD_AMBIENT_LIGHT_SHADER_H

#include "Shader.h"
#include "LightEmitter.h"

class ForwardAmbientLightShader : public Shader {
public:
	ForwardAmbientLightShader(const std::string& fileName) : Shader(fileName) {}

	void update(Matrix4f& matrix, LightEmitter& lightEmitter) {
		glUniformMatrix4fv(uniforms["transform"], 1, GL_FALSE, &(matrix(0, 0)));
		glUniform1f(uniforms["lightIntensity"], lightEmitter.intensity);
		glUniform4fv(uniforms["lightColor"], 1, &(lightEmitter.color.x()));
	}

	void addUniforms() {
		addUniform("transform");
		addUniform("lightIntensity");
		addUniform("lightColor");
	}
};

#endif /* defined(FORWARD_AMBIENT_LIGHT_SHADER_H) */
