#ifndef SKY_BOX_SHADER_H
#define SKY_BOX_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"

class SkyBoxShader : public InstancedShader<SkyBoxShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/util/skyBox");
	}

	void update(Matrix4f& vp)
	{
		glUniformMatrix4fv(uniforms["vp"], 1, GL_FALSE, &(vp(0, 0)));
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}

	Type getType() override
	{
		return Shader::Type::CUSTOM_SHADER;
	}
};

#endif // SKY_BOX_SHADER_H
