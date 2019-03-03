#ifndef DOF_SHADER_CPP
#define DOF_SHADER_CPP

#include <resource/Shader.h>

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class DOFShader : public InstancedShader<DOFShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/dof");
	}

	void addUniforms() override
	{
		addUniform("depth");

		addTexture("renderTexture");
		addTexture("gPosition");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // DOF_SHADER_CPP