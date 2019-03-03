#ifndef FXAA_SHADER_CPP
#define FXAA_SHADER_CPP

#include <resource/Shader.h>

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class FXAAShader : public InstancedShader<FXAAShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/fxaa");
	}

	void addUniforms() override
	{
		addUniform("screenWidth");
		addUniform("screenHeight");
		addUniform("R_fxaaSpanMax");
		addUniform("R_fxaaReduceMin");
		addUniform("R_fxaaReduceMul");

		addTexture("renderTexture");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // FXAA_SHADER_CPP