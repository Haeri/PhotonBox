#ifndef BLUR_H_SHADER_H
#define BLUR_H_SHADER_H

#include "../Resources/Shader.h"
#include "../Resources/Vertex.h"

class BlurHShader : public Shader {
public:
	BlurHShader(const std::string& fileName) { init(fileName); }

	void addUniforms() override {
		addUniform("offset");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // BLUR_H_SHADER_H