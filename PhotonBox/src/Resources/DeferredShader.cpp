#ifndef DEFERRED_SHADER_CPP
#define DEFERRED_SHADER_CPP

#include "../Components/Camera.h"
#include "Shader.h"
#include "Resources.h"

class DeferredShader : public InstancedShader<DeferredShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/deferre-rendering/deferredShader");
	}

	void update(Transform* transform) override
	{
		glUniform3fv(uniforms["viewPos"], 1, &(Camera::getMainCamera()->transform->getPositionWorld().x()));
	}

	void addUniforms() override
	{
		addUniform("viewPos");
		
		for (size_t i = 0; i < 3; i++)
		{
			addUniform("lights[" + std::to_string(i) + "].Position");
			addUniform("lights[" + std::to_string(i) + "].Color");
			addUniform("lights[" + std::to_string(i) + "].Linear");
			addUniform("lights[" + std::to_string(i) + "].Quadratic");
		}

		addTexture("gPosition");
		addTexture("gNormal");
		addTexture("gRoughness");
		addTexture("gMetallic");
		addTexture("gAlbedo");
	}

	void addAttributes() override
	{
		addAttribut("aPos", Vertex::AttibLocation::POSITION);
	}
};

#endif // DEFERRED_SHADER_CPP