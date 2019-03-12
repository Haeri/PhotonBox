#ifndef TONE_MAPPING_SHADER_CPP
#define TONE_MAPPING_SHADER_CPP

#include <resource/Shader.h>

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class ToneMappingShader : public InstancedShader<ToneMappingShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/tonemapping");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // TONE_MAPPING_SHADER_CPP