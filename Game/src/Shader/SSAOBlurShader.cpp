#ifndef SSAO_BLUR_SHADER_CPP
#define SSAO_BLUR_SHADER_CPP

#include <PhotonBox/resource/Shader.h>

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class SSAOBlurShader : public InstancedShader<SSAOBlurShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/ssaoBlur");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // SSAO_BLUR_SHADER_CPP