#include <algorithm>
#include <string>
#include <iostream>
#include "Renderer.h"
#include "../Display.h"
#include "../../Components/ObjectRenderer.h"
#include "../../Components/Transform.h"
#include "../../Resources/Skybox.h"
#include "../src/Core/Systems/Lighting.h"
#include "../src/Components/Camera.h"
#include "../src/Components/PointLight.h"
#include "../src/Components/AmbientLight.h"
#include "../src/Components/DirectionalLight.h"
#include "../src/Components/MeshRenderer.h"

bool Renderer::_isDebug;
SkyBox Renderer::_skyBox;
std::vector<ObjectRenderer*> Renderer::_renderQueue;

ForwardAmbientLightShader* Renderer::_ambientLightShader;
//ForwardDirectionalLightShader* Renderer::_directionalLightShader;
//ForwardPointLightShader* Renderer::_pointLightShader;

void Renderer::addToRenderQueue(ObjectRenderer *objectRenderer) {
	_renderQueue.push_back(objectRenderer);
}

void Renderer::removeFromRenderQueue(ObjectRenderer *objectRenderer) {
	_renderQueue.erase(std::remove(_renderQueue.begin(), _renderQueue.end(), objectRenderer), _renderQueue.end());
}

void Renderer::setSkyBox(CubeMap* cubeMap){
	_skyBox.setLightProbe(new LightProbe(cubeMap));
}

void Renderer::init() {
	// OpenGL config
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	_ambientLightShader = ForwardAmbientLightShader::getInstance();
	//_directionalLightShader = ForwardDirectionalLightShader::getInstance();
	//_pointLightShader = ForwardPointLightShader::getInstance();

	_isDebug = false;
}

void Renderer::start() {
	_skyBox.init();

	for (std::vector<ObjectRenderer*>::iterator it = _renderQueue.begin(); it != _renderQueue.end(); ++it) {
		(*it)->init();
	}
}

void Renderer::render() {
	Display::clearDisplay(0.3, 0.3, 0.3, 1);

	_skyBox.render();

	for (std::vector<ObjectRenderer*>::iterator it = _renderQueue.begin(); it != _renderQueue.end(); ++it) {
		if ((*it)->getEnable()) {
			if(typeid((**it)) != typeid(MeshRenderer) || ((*it)->getMaterial() != nullptr && (*it)->getMaterial()->shader != nullptr)) {
				glEnable(GL_DEPTH_TEST);
				(*it)->render();
			}else {
				glEnable(GL_DEPTH_TEST);

				// IBL
				_skyBox.getLightProbe()->getIrradianceCube()->bind(_ambientLightShader->textures["irradianceMap"].unit);
				_skyBox.getLightProbe()->getSpecularCube()->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);

				// AMBIENT
				AmbientLight* ambient = Lighting::getLights<AmbientLight>()[0];
				(*it)->render(_ambientLightShader, ambient);


				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				glDepthMask(GL_FALSE);
				glDepthFunc(GL_EQUAL);



				// DIRECTIONAL LIGHTS
				std::unordered_map<std::type_index, std::vector<LightEmitter*>> lights = Lighting::getAllLights();
				for (auto const &lightvec : lights) {
					for (auto const &light : lightvec.second) {
						if (!light->getEnable()) continue;
						(*it)->render(light->getLightShader(), light);
					}
				}


	/*			for (size_t i = 0; i < directionalLights.size(); ++i) {
					if (!directionalLights[i]->getEnable()) continue;
					(*it)->render(directionalLights[i]->getLightShader(), directionalLights[i]);
				}*/

				//// POINT LIGHTS
				//std::vector<PointLight*> pointLights = Lighting::getLights<PointLight>();
				//for (size_t i = 0; i < pointLights.size(); ++i) {
				//	if (!pointLights[i]->getEnable()) continue;
				//	(*it)->render(_pointLightShader, pointLights[i]);
				//}

				// SPOT LIGHTS

				glDepthMask(GL_TRUE);
				glDepthFunc(GL_LESS);
				glDisable(GL_BLEND);

			}
		}
	}
	
	if(_isDebug){
		for (std::vector<ObjectRenderer*>::iterator it = _renderQueue.begin(); it != _renderQueue.end(); ++it) {
			if ((*it)->getEnable()) {
				(*it)->transform->renderHandels();
			}
		}
	}

	Display::swapBuffer();
}

void Renderer::setDebug(bool debug) {
	_isDebug = debug;
}

void Renderer::destroy() {
	_renderQueue.clear();
}

void Renderer::printList(){
	for (std::vector<ObjectRenderer*>::iterator it = _renderQueue.begin(); it != _renderQueue.end(); ++it) {
		std::cout << (*it)->getName() << std::endl;
	}
}