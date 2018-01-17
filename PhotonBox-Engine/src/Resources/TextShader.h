#ifndef TEXT_SHADER_H
#define TEXT_SHADER_H

#include "Shader.h"
#include "Vertex.h"
#include "../Core/Display.h"

class TextShader : public InstancedShader<TextShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/ui/text");
	}

	void update()
	{
		Matrix4f ortho = Matrix4f::createOrthographic(0, Display::getWidth(), 0, Display::getHeight());
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
};

#endif // TEXT_SHADER_H