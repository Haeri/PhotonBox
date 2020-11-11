#ifndef UNLIT_SHADER_H
#define UNLIT_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"

class UnlitShader : public InstancedShader<UnlitShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/util/unlitShader");
	}

	void update(Transform* transform) override
	{
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();

		setUniform("mvp", mvp);
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

#endif // UNLIT_SHADER_H