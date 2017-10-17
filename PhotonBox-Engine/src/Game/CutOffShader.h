#ifndef CUT_OFF_SHADER_H
#define CUT_OFF_SHADER_H

#include "../Resources/Shader.h"
#include "../Resources/Vertex.h"

class CutOffShader : public InstancedShader<CutOffShader>{
public:
	std::string getFilePath() override {
		return std::string("./res/post-processing/cutOff");
	}

	void addUniforms() override {
		addUniform("threshold");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // CUT_OFF_SHADER_H