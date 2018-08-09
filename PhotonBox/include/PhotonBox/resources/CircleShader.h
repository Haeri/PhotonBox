#ifndef CIRCLE_SHADER_H
#define CIRCLE_SHADER_H

#include "PhotonBox/resources/Shader.h"

class CircleShader : public InstancedShader<CircleShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/util/circle");
	}

	void addUniforms() override
	{
		addUniform("mvp");
		//addUniform("color");

		//addTexture("texture");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("color", Vertex::AttibLocation::COLOR);
	}

	void update(Transform* transform) override
	{
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection();
		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
	}
};

#endif // CIRCLE_SHADER_H
