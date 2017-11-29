#ifndef AUTO_EXPOSURE_SHADER_H
#define AUTO_EXPOSURE_SHADER_H

#include "Shader.h"
#include "Vertex.h"

class AutoExposureShader : public InstancedShader<AutoExposureShader> {
public:
	std::string getFilePath() override {
		return std::string("./res/post-processing/autoexposure");
	}

	void addUniforms() override {
		addUniform("maxMip");

		addTexture("renderTexture");
		addTexture("luminanceSample");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // AUTO_EXPOSURE_SHADER_H