#ifndef BLUR_V_SHADER_H
#define BLUR_V_SHADER_H

#include "../Resources/Shader.h"
#include "../Resources/Vertex.h"

class BlurVShader : public InstancedShader<BlurVShader> {
public:
	std::string getFilePath() override {
		return std::string("./res/post-processing/blur-v");
	}

	void addUniforms() override {
		addUniform("offset");
		addTexture("renderTexture");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // BLUR_V_SHADER_H