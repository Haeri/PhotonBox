#ifndef ADD_SHADER_H
#define ADD_SHADER_H

#include "../Resources/Shader.h"
#include "../Resources/Vertex.h"

class AddShader : public Shader {
public:
	AddShader(const std::string& fileName) { init(fileName); }

	void addUniforms() override {
		addTexture("renderTexture");
		addTexture("renderTexture2");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // ADD_SHADER_H