#ifndef FORWARD_SHADER_H
#define FORWARD_SHADER_H

#include "ForwardAmbientLightShader.h"
#include "ForwardPointLightShader.h"
#include "ForwardDirectionalLightShader.h"

class ForwardShader {
public:
	ForwardAmbientLightShader* ambientLightShader;
	ForwardDirectionalLightShader* directionalLightShader;
	ForwardPointLightShader* pointLightShader;

	ForwardShader() {
		ambientLightShader = new ForwardAmbientLightShader(ambientLight);
		directionalLightShader = new ForwardDirectionalLightShader(directionalLight);
		pointLightShader = new ForwardPointLightShader(pointLight);
	}

	void bindAmbientShader() {
		ambientLightShader->bind();
	}

	void bindDirectionalLightShader() {
		directionalLightShader->bind();
	}

	void bindPointLightShader() {
		pointLightShader->bind();
	}

private:
	const std::string SHADER_DIR		= "./res/forward-rendering/";
	const std::string ambientLight		= SHADER_DIR + "forward_ambientlight";
	const std::string directionalLight	= SHADER_DIR + "forward_directionallight";
	const std::string pointLight		= SHADER_DIR + "forward_pointlight";
};

#endif /* defined(FORWARD_SHADER_H) */
