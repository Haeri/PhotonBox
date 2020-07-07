#ifndef SSREFLECTION_SHADER_CPP
#define SSREFLECTION_SHADER_CPP

#include <PhotonBox/component/Camera.h>
#include <PhotonBox/resource/Shader.h>

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class SSReflectionShader : public InstancedShader<SSReflectionShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/ssReflection");
	}

	void update(Transform* transform) override
	{
		setUniform("projection", Camera::getMainCamera()->getProjectionMatrix());	
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // SSREFLECTION_SHADER_CPP