#ifndef SPECULAR_CONVOLUTION_SHADER_H
#define SPECULAR_CONVOLUTION_SHADER_H

#include "Shader.h"
#include "Vertex.h"

class SpecularConvolutionShader : public InstancedShader<SpecularConvolutionShader> {
public:
	std::string getFilePath() override {
		return std::string("./res/util/specular-convolution");
	}

	//void update(Matrix4f& vp) {
	//	glUniformMatrix4fv(uniforms["vp"], 1, GL_FALSE, &(vp(0, 0)));
	//}

	void addUniforms() override {
		addUniform("projection");
		addUniform("view");
		addUniform("roughness");

		addTexture("environmentMap");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // SPECULAR_CONVOLUTION_SHADER_H