#ifndef BLUR_SHADER_CPP
#define BLUR_SHADER_CPP

#include <PhotonBox/resource/Shader.h>

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class BlurShader : public InstancedShader<BlurShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/blur");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // BLUR_SHADER_CPP