#ifndef TEXT_SHADER_H
#define TEXT_SHADER_H

#include "PhotonBox/core/Display.h"
#include "PhotonBox/resources/Shader.h"
#include "PhotonBox/resources/Resources.h"

class TextShader : public InstancedShader<TextShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/ui/text");
	}

	void update()
	{
		Matrix4f ortho = Matrix4f::createOrthographic(0.0f, static_cast<float>(Display::getWidth()), 0.0f, static_cast<float>(Display::getHeight()));
		glUniformMatrix4fv(uniforms["projection"], 1, GL_FALSE, &(ortho(0, 0)));
	}

	void addUniforms() override
	{
		addUniform("projection");
		addUniform("textColor");

		addTexture("text");
	}

	void addAttributes() override
	{
		addAttribut("vertex", Vertex::AttibLocation::POSITION);
	}

	Type getType() override
	{
		return Shader::Type::CUSTOM_SHADER;
	}
};

#endif // TEXT_SHADER_H
