#ifndef FORWARD_SHADER_H
#define FORWARD_SHADER_H

#include "Shader.h"

#include "ForwardAmbientLightShader.h"
#include "ForwardPointLightShader.h"

class ForwardShader: public Shader {
public:
	ForwardAmbientLightShader* ambientShader;
	ForwardPointLightShader* pointLightShader;

	ForwardShader(const std::string& ambient, const std::string& pointLight) {
		ambientShader = new ForwardAmbientLightShader(ambient);
		pointLightShader = new ForwardPointLightShader(pointLight);
	}

	void bindAmbientShader() {
		ambientShader->bind();
	}

	void bindPointLightShader() {
		pointLightShader->bind();
	}
};

#endif /* defined(FORWARD_SHADER_H) */
