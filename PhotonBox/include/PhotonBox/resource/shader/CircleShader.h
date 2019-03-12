#ifndef CIRCLE_SHADER_H
#define CIRCLE_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"

class CircleShader : public InstancedShader<CircleShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/util/circle");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("color", Vertex::AttibLocation::COLOR);
	}

	void update(Transform* transform) override
	{
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection();
		
		setUniform("mvp", mvp);
	}
};

#endif // CIRCLE_SHADER_H
