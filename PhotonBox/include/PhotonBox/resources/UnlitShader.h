#ifndef UNLIT_SHADER_H
#define UNLIT_SHADER_H

#include "PhotonBox/resources/Shader.h"

class UnlitShader : public InstancedShader<UnlitShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/util/unlitShader");
	}

	void addUniforms() override
	{
		addUniform("mvp");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}

	void update(Transform* transform) override
	{
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
	}

	Type getType() override
	{
		return Shader::Type::CUSTOM_SHADER;
	}
};

#endif // UNLIT_SHADER_H