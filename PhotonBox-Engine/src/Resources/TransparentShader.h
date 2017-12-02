#ifndef TRANSPARENT_SHADER_H
#define TRANSPARENT_SHADER_H

#include "Shader.h"
#include "Vertex.h"
#include "../Components/Camera.h"

class TransparentShader : public InstancedShader<TransparentShader> {
public:
	std::string getFilePath() override {
		return std::string("./res/PBS/transparent");
	}

	void update(Transform* transform, LightEmitter* light) {
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		Vector4f eyePos = Vector4f(Camera::getMainCamera()->transform->getPositionWorld(), 1);
		AmbientLight* al = dynamic_cast<AmbientLight*>(light);

		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
		glUniformMatrix4fv(uniforms["modelMatrix"], 1, GL_FALSE, &(transform->getTransformationMatrix()(0, 0)));
		glUniform3fv(uniforms["viewPos"], 1, &(eyePos.x()));
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}

	void addUniforms() override {
		addUniform("mvp");
		addUniform("modelMatrix");
		addUniform("viewPos");

		addUniform("tint");
		addUniform("minBound");
		addUniform("maxBound");
		addUniform("boundPos");
		addUniform("useCorrection");

		addTexture("convolutedSpecularMap");

		addTexture("albedoMap");
		addTexture("normalMap");
		addTexture("emissionMap");
		addTexture("roughnessMap");
	}
};

#endif // TRANSPARENT_SHADER_H
