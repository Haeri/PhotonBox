#ifndef EXPOSURE_SHADER_CPP
#define EXPOSURE_SHADER_CPP

#include <resource/Shader.h>

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class ExposureShader : public InstancedShader<ExposureShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/exposure");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // EXPOSURE_SHADER_CPP