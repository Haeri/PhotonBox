#ifndef DEPTH_SHADER_H
#define DEPTH_SHADER_H

#include "Shader.h"
#include "Vertex.h"
#include "../Components/Transform.h"
#include "../Components/DirectionalLight.h"

class DepthShader : public InstancedShader<DepthShader> {
public:
	std::string getFilePath() override {
		return std::string("./res/util/depth");
	}

	void update(Transform* transform, LightEmitter* light) {
		Matrix4f model = transform->getTransformationMatrix();
		DirectionalLight* dl = dynamic_cast<DirectionalLight*>(light);
		/*Matrix4f lightView = Matrix4f::lookAt(
			Camera::getMainCamera()->transform->getPosition() + (dl->direction * -3),
			Vector3f(0.0f, 1.0f, 0.0f),
			dl->direction);*/
		Matrix4f lightView = Matrix4f::lookAt(
			(dl->direction * -3),
			Vector3f(0.0f, 1.0f, 0.0f),
			dl->direction);
		Matrix4f lightSpaceMatrix = dl->lightProjection * lightView;

		glUniformMatrix4fv(uniforms["lightSpaceMatrix"], 1, GL_FALSE, &(lightSpaceMatrix(0, 0)));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, &(model(0, 0)));
	}

	void addUniforms() override {
		addUniform("lightSpaceMatrix");
		addUniform("model");
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // DEPTH_SHADER_H