#ifndef BLUR_V_SHADER_CPP
#define BLUR_V_SHADER_CPP

#include <resource/Shader.h>

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class BlurVShader : public InstancedShader<BlurVShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/blur-v");
	}

	void addUniforms() override
	{
		addUniform("offset");
		addTexture("renderTexture");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // BLUR_V_SHADER_CPP