#ifndef SSAO_SHADER_CPP
#define SSAO_SHADER_CPP

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Display.h"
#include "PhotonBox/component/Camera.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class SSVOShader : public InstancedShader<SSVOShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/deferred-rendering/ssvo");
	}

	void update(Transform* transform) override
	{
		setUniform("screenWidth", Display::getWidth() / 2.0f);
		setUniform("screenHeight", Display::getHeight() / 2.0f);
		setUniform("projection", Camera::getMainCamera()->getProjectionMatrix());
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // SSAO_SHADER_CPP