#ifndef BASIC_SHADER_H
#define BASIC_SHADER_H

#include "Shader.h"
#include "Vertex.h"

class BasicShader : public InstancedShader<BasicShader> {
public:
	std::string getFilePath() override {
		return std::string("./res/basicShader");
	}

	void update(Transform* transform) override{
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();

		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
	}
	
	void addUniforms() override {
		addUniform("mvp");

		addTexture("albedoMap");
	}
	
	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
	}
};

#endif // BASIC_SHADER_H
