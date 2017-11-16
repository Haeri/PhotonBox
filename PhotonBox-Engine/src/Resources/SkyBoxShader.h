#ifndef SKY_BOX_SHADER_H
#define SKY_BOX_SHADER_H

#include "Shader.h"
#include "Vertex.h"

class SkyBoxShader : public InstancedShader<SkyBoxShader>{
public:
	std::string getFilePath() override {
		return std::string("./res/forward-rendering/skyBox");
	}
	
	void update(Matrix4f& vp) {
		glUniformMatrix4fv(uniforms["vp"], 1, GL_FALSE, &(vp(0, 0)));
	}

	void addUniforms() override {
		addUniform("vp");
		addUniform("mip");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // SKY_BOX_SHADER_H