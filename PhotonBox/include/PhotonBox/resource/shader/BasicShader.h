#ifndef BASIC_SHADER_H
#define BASIC_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"

class BasicShader : public InstancedShader<BasicShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/util/basicShader");
	}

	void update(Transform* transform) override
	{
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();

		setUniform("mvp", mvp);
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
	}
};

#endif // BASIC_SHADER_H
