#ifndef SSREFLECTION_SHADER_CPP
#define SSREFLECTION_SHADER_CPP

#include <Components/Camera.h>
#include <Resources/Shader.h>

class SSReflectionShader : public InstancedShader<SSReflectionShader>
{
public:
	std::string getFilePath() override
	{
		return std::string("./res/shaders/post-processing/ssReflection");
	}

	void update(Transform* transform) override
	{
		glUniformMatrix4fv(uniforms["projection"], 1, GL_FALSE, &(Camera::getMainCamera()->getProjectionMatrix()(0, 0)));
	}

	void addUniforms() override
	{
		addUniform("projection");

		addTexture("mainBuffer");
		addTexture("gPosition");
		addTexture("gNormal");
		addTexture("gMetallic");
		addTexture("gRoughness");
	}

	void addAttributes() override
	{
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // SSREFLECTION_SHADER_CPP