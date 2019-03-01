#ifndef TAA_SHADER_CPP
#define TAA_SHADER_CPP

#include <Resources/Shader.h>

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class TAAShader : public InstancedShader<TAAShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/TAA");
	}

	void addUniforms() override
	{
		addTexture("lowResTexture");
		addTexture("previousLowResTexture");
		addTexture("gPosition");
		addTexture("normalVelocity");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // TAA_SHADER_CPP