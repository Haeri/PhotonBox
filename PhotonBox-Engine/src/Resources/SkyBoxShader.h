#ifndef SKY_BOX_SHADER_H
#define SKY_BOX_SHADER_H

#include "Shader.h"
#include "Vertex.h"

class SkyBoxShader : public Shader {
public:
	SkyBoxShader(const std::string& fileName) { init(fileName); }

	void update(Matrix4f& vp) {
		glUniformMatrix4fv(uniforms["vp"], 1, GL_FALSE, &(vp(0, 0)));
	}

	void addUniforms() override {
		addUniform("vp");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // SKY_BOX_SHADER_H