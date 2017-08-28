#ifndef LIT_SHADER_H
#define LIT_SHADER_H

#include "Shader.h"

class LitShader : public Shader {
public:
	LitShader(const std::string& fileName) { init(fileName); }

	void addUniforms() override {
		addUniform("transform");
		addUniform("color");
	}

	void addAttributes() override {
		addAttribut("position");
	}

	void update(Matrix4f& matrix, Vector3f color) {
		glUniformMatrix4fv(uniforms["transform"], 1, GL_FALSE, &(matrix(0, 0)));
		glUniform3fv(uniforms["color"], 1, &(color.x()));
	}
};

#endif // LIT_SHADER_H
