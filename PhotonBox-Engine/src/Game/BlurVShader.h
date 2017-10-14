#ifndef BLUR_V_SHADER_H
#define BLUR_V_SHADER_H

#include "../Resources/Shader.h"
#include "../Resources/Vertex.h"

class BlurVShader : public Shader {
public:
	BlurVShader(const std::string& fileName) { init(fileName); }

	void addUniforms() override {
		addUniform("offset");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // BLUR_V_SHADER_H