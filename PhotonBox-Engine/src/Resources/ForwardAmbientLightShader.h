#ifndef FORWARD_AMBIENT_LIGHT_SHADER_H
#define FORWARD_AMBIENT_LIGHT_SHADER_H

#include "Shader.h"
#include "../Components/LightEmitter.h"
#include "Vertex.h"
#include "../Components/Camera.h"
#include "../Components/AmbientLight.h"

class ForwardAmbientLightShader : public InstancedShader<ForwardAmbientLightShader>{
public:
	std::string getFilePath() override {
		return std::string("./res/shaders/forward-rendering/base");
	}

	void update(Transform* transform, LightEmitter* light){
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		Vector4f eyePos = Vector4f(Camera::getMainCamera()->transform->getPositionWorld(), 1);
		AmbientLight* al = dynamic_cast<AmbientLight*>(light);

		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
		glUniformMatrix4fv(uniforms["modelMatrix"], 1, GL_FALSE, &(transform->getTransformationMatrix()(0, 0)));
		glUniform1f(uniforms["light.intensity"], al->intensity);
		glUniform3fv(uniforms["light.color"], 1, &(al->color.x()));
		glUniform3fv(uniforms["viewPos"], 1, &(eyePos.x()));
	}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}

	void addUniforms() override{
		addUniform("mvp");
		addUniform("modelMatrix");
		addUniform("viewPos");
		addUniform("light.intensity");
		addUniform("light.color");
		addUniform("light.constant");
		addUniform("light.linear");
		addUniform("light.quadratic");

		addUniform("minBound");
		addUniform("maxBound");
		addUniform("boundPos");
		addUniform("useCorrection");

		addTexture("irradianceMap"); 
		addTexture("convolutedSpecularMap");
		
		addTexture("albedoMap");
		addTexture("normalMap");
		addTexture("aoMap");
		addTexture("emissionMap");
		addTexture("roughnessMap");
		addTexture("metallicMap");
	}
};

#endif /* defined(FORWARD_AMBIENT_LIGHT_SHADER_H) */
