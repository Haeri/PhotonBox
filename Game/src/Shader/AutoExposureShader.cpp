#ifndef AUTO_EXPOSURE_SHADER_CPP
#define AUTO_EXPOSURE_SHADER_CPP

#include <resource/Shader.h>
#include <core/Time.h>

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class AutoExposureShader : public InstancedShader<AutoExposureShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/autoexposure");
	}

	void update(Transform* t) override
	{
		glUniform1f(uniforms["delteTime"], Time::deltaTimef);
	}

	void addUniforms() override
	{
		addUniform("maxMip");
		addUniform("minLum");
		addUniform("maxLum");
		addUniform("delteTime");
		addUniform("adaptationSpeed");

		addTexture("luminanceSampleCurrent");
		addTexture("luminanceSampleLast");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // AUTO_EXPOSURE_SHADER_CPP