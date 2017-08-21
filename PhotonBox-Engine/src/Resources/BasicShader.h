#ifndef BASIC_SHADER_H
#define BASIC_SHADER_H

#include "Shader.h"

class BasicShader: public Shader {
public:
	BasicShader(const std::string& fileName) { init(fileName); }

	void addUniforms() override {
		addUniform("transform");
	}
	
	void addAttributes() override {
		addAttribut("position");
		addAttribut("normal");
		addAttribut("color");
		addAttribut("uv");
	}
	
	void update(Matrix4f& matrix) override {
		glUniformMatrix4fv(uniforms["transform"], 1, GL_FALSE, &(matrix(0, 0)));
	}
};

#endif // BASIC_SHADER_H
