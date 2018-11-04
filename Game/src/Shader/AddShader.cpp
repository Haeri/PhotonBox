#ifndef ADD_SHADER_CPP
#define ADD_SHADER_CPP

#include <Resources/Shader.h>

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class AddShader : public InstancedShader<AddShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/add");
	}

	void addUniforms() override
	{
		addTexture("renderTexture");
		addTexture("renderTexture2");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // ADD_SHADER_CPP