#ifndef LIT_SHADER_H
#define LIT_SHADER_H

#include "Shader.h"

class LitShader : public InstancedShader<LitShader>{
public:
	std::string getFilePath() override {
		return std::string("./res/litShader");
	}

	void addUniforms() override {
		addUniform("mvp");
		addUniform("color");

		addTexture("texture");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
	}

	void update(Transform* transform) override {
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
	}
};

#endif // LIT_SHADER_H