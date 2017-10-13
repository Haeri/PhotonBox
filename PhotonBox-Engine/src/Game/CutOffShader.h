#ifndef CUT_OFF_SHADER_H
#define CUT_OFF_SHADER_H

#include "../Resources/Shader.h"
#include "../Resources/Vertex.h"

class CutOffShader : public Shader {
public:
	CutOffShader(const std::string& fileName) { init(fileName); }

	void addUniforms() override {
		addUniform("threshold");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // CUT_OFF_SHADER_H