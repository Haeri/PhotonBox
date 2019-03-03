#ifndef DEPTH_SHADER_H
#define DEPTH_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"

class DepthShader : public InstancedShader<DepthShader>
{
public:

	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/util/depth");
	}

	void update(Transform* transform)
	{
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
	}

	void addUniforms() override
	{
		addUniform("mvp");

		addTexture("albedoMap");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
	}

	Type getType() override
	{
		return Shader::Type::CUSTOM_SHADER;
	}
};

#endif // DEPTH_SHADER_H
