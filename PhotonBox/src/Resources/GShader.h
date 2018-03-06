#ifndef G_SHADER_H
#define G_SHADER_H

#include "Shader.h"
#include "Vertex.h"

class GShader : public InstancedShader<GShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/deferre-rendering/gShader");
	}

	void update(Transform* transform)
	{
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, &(transform->getTransformationMatrix()(0, 0)));
		glUniformMatrix4fv(uniforms["view"], 1, GL_FALSE, &(Camera::getMainCamera()->getViewMatrix()(0, 0)));
		glUniformMatrix4fv(uniforms["projection"], 1, GL_FALSE, &(Camera::getMainCamera()->getProjectionMatrix()(0, 0)));
	}

	void addUniforms() override
	{
		addUniform("model");
		addUniform("view");
		addUniform("projection");

		addTexture("albedoMap");
		addTexture("normalMap");
		addTexture("roughnessMap");
		addTexture("metallicMap");
	}

	void addAttributes() override
	{
		addAttribut("aPos", Vertex::AttibLocation::POSITION);
		addAttribut("aNormal", Vertex::AttibLocation::NORMAL);
		addAttribut("aTexCoords", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}
};

#endif // G_SHADER_H
