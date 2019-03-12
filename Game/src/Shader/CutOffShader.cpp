#ifndef CUT_OFF_SHADER_CPP
#define CUT_OFF_SHADER_CPP

#include <resource/Shader.h>

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class CutOffShader : public InstancedShader<CutOffShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/cutOff");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // CUT_OFF_SHADER_CPP