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
#include "../src/Components/SpotLight.h"
#include "../src/Components/AmbientLight.h"
#include "../src/Components/DirectionalLight.h"
#include "../src/Components/MeshRenderer.h"
#include "../../Core/FrameBuffer.h"
#include "../PostProcessor.h"
#include "../../Resources/LitShader.h"
#include "../../Resources/DepthShader.h"
#include "../Systems/SceneManager.h"
#include "../../Resources/Scene.h"
#include "../../Resources/TransparentShader.h"

bool Renderer::_isDebug;
SkyBox Renderer::_skyBox;
std::vector<ObjectRenderer*> Renderer::_renderListOpaque;
std::vector<ObjectRenderer*> Renderer::_renderListTransparent;
std::map<float, ObjectRenderer*> Renderer::_renderQueueTransparent;

ForwardAmbientLightShader* Renderer::_ambientLightShader;
ForwardDirectionalLightShader* Renderer::_directionalLightShader;
ForwardPointLightShader* Renderer::_pointLightShader;
ForwardSpotLightShader* Renderer::_spotLightShader;
TransparentShader* Renderer::_transparentBaseShader;

FrameBuffer* Renderer::_mainFrameBuffer;
Vector3f Renderer::_clearColor = Vector3f(0.3, 0.3, 0.3);

void Renderer::addToRenderQueue(ObjectRenderer *objectRenderer, bool isOpaque) {
	if (isOpaque) {
		_renderListOpaque.push_back(objectRenderer);
	}else {
		_renderListTransparent.push_back(objectRenderer);
	}
}

void Renderer::removeFromRenderQueue(ObjectRenderer *objectRenderer) {
	if (objectRenderer->isOpaque()) {
		_renderListOpaque.erase(std::remove(_renderListOpaque.begin(), _renderListOpaque.end(), objectRenderer), _renderListOpaque.end());
	}else {
		_renderListTransparent.erase(std::remove(_renderListTransparent.begin(), _renderListTransparent.end(), objectRenderer), _renderListTransparent.end());
	}	
}

void Renderer::setSkyBox(CubeMap* cubeMap){
	_skyBox.setLightProbe(new LightProbe(cubeMap));
}

void Renderer::init() {
	init(1);
}

void Renderer::init(int superSampling) {
	// OpenGL config
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glClearColor(_clearColor.x(), _clearColor.y(), _clearColor.z(), 1);
	
	_ambientLightShader = ForwardAmbientLightShader::getInstance();
	_directionalLightShader = ForwardDirectionalLightShader::getInstance();
	_pointLightShader = ForwardPointLightShader::getInstance();
	_spotLightShader = ForwardSpotLightShader::getInstance();
	_transparentBaseShader = TransparentShader::getInstance();
	_mainFrameBuffer = new FrameBuffer(Display::getWidth()*superSampling, Display::getHeight()*superSampling);

	_isDebug = false;
}

void Renderer::start() {
	_skyBox.init();

	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it) {
		(*it)->init();
	}
	for (std::vector<ObjectRenderer*>::iterator it = _renderListTransparent.begin(); it != _renderListTransparent.end(); ++it) {
		(*it)->init();
	}
}

void Renderer::render() {
	render(false);
}

void Renderer::renderShadows(bool captureMode) {
	glCullFace(GL_FRONT);
	std::vector<DirectionalLight*> directionalLights = Lighting::getLights<DirectionalLight>();
	for (size_t i = 0; i < directionalLights.size(); ++i) {
		if (!directionalLights[i]->getEnable()) continue;
		directionalLights[i]->renderShadowMap(captureMode);
	}
	glCullFace(GL_BACK);
}

void Renderer::render(bool captureMode) {
	render(captureMode, nullptr);
}

void Renderer::render(bool captureMode, LightMap* lightmap) {
	if (!captureMode) {
		// Bind & clear Shadow FBO
		renderShadows(false);

		// Bind & clear Main FBO
		_mainFrameBuffer->enable();
		_mainFrameBuffer->clear();
	}

	_skyBox.render();

	
	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it) {
		if ((*it)->getEnable() && Camera::getMainCamera()->frustumTest(*it)) {
			if (!(*it)->captureble && captureMode) continue;
			
			
			if (typeid((**it)) != typeid(MeshRenderer) || ((*it)->getMaterial() != nullptr && (*it)->getMaterial()->shader != nullptr)) {
				glEnable(GL_DEPTH_TEST);
				(*it)->render();
			}else {
				glEnable(GL_DEPTH_TEST);


				// IBL
				_ambientLightShader->bind();
				LightProbe* lp = Lighting::findInLightProberVolume((*it)->transform);
				if (!captureMode && lp != nullptr) {
					lp->getIrradianceCube()->bind(_ambientLightShader->textures["irradianceMap"].unit);
					lp->getSpecularCube()->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);

					_ambientLightShader->setUniform("minBound", lp->bounds.getMinBoundGlobal());
					_ambientLightShader->setUniform("maxBound", lp->bounds.getMaxBoundGlobal());
					_ambientLightShader->setUniform("boundPos", lp->bounds.getBoundPosition());
					_ambientLightShader->setUniform("useCorrection", true);
				}else if(lightmap != nullptr){
					lightmap->irradianceMap->bind(_ambientLightShader->textures["irradianceMap"].unit);
					lightmap->specularMap->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);
					_ambientLightShader->setUniform("useCorrection", false);
				}else {
					_skyBox.getLightProbe()->getIrradianceCube()->bind(_ambientLightShader->textures["irradianceMap"].unit);
					_skyBox.getLightProbe()->getSpecularCube()->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);
					_ambientLightShader->setUniform("useCorrection", false);
				}

				// AMBIENT
				AmbientLight* ambient = Lighting::getLights<AmbientLight>()[0];
				(*it)->render(_ambientLightShader, ambient);


				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				glDepthMask(GL_FALSE);
				glDepthFunc(GL_EQUAL);

				std::unordered_map<std::type_index, std::vector<LightEmitter*>> lights = Lighting::getAllLights();
				for (auto const &lightvec : lights) {
					for (auto const &light : lightvec.second) {
						if (!light->getEnable() || (typeid(*(light->getLightShader())) == typeid(*(ForwardAmbientLightShader::getInstance())))) continue;
						(*it)->render(light->getLightShader(), light);
					}
				}

				glDepthMask(GL_TRUE);
				glDepthFunc(GL_LESS);
				glDisable(GL_BLEND);

			}
		}
	}
	
	
	if(!captureMode){
		updateTransparentQueue();
		for (std::map<float, ObjectRenderer*>::reverse_iterator it = _renderQueueTransparent.rbegin(); it != _renderQueueTransparent.rend(); ++it) {
				glEnable(GL_DEPTH_TEST);

				// IBL
				_transparentBaseShader->bind();
				LightProbe* lp = Lighting::findInLightProberVolume(it->second->transform);
				if (!captureMode && lp != nullptr) {
					lp->getSpecularCube()->bind(_transparentBaseShader->textures["convolutedSpecularMap"].unit);

					_transparentBaseShader->setUniform("minBound", lp->bounds.getMinBoundGlobal());
					_transparentBaseShader->setUniform("maxBound", lp->bounds.getMaxBoundGlobal());
					_transparentBaseShader->setUniform("boundPos", lp->bounds.getBoundPosition());
					_transparentBaseShader->setUniform("useCorrection", true);
				}
				else {
					_skyBox.getLightProbe()->getSpecularCube()->bind(_transparentBaseShader->textures["convolutedSpecularMap"].unit);
					_transparentBaseShader->setUniform("useCorrection", false);
				}


				// AMBIENT
				AmbientLight* ambient = Lighting::getLights<AmbientLight>()[0];

				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				it->second->render(_transparentBaseShader, ambient);


				glDepthMask(GL_FALSE);
				glDepthFunc(GL_EQUAL);

				std::unordered_map<std::type_index, std::vector<LightEmitter*>> lights = Lighting::getAllLights();
				for (auto const &lightvec : lights) {
					for (auto const &light : lightvec.second) {
						if (!light->getEnable() || (typeid(*(light->getLightShader())) == typeid(*(ForwardAmbientLightShader::getInstance())))) continue;
						it->second->render(light->getLightShader(), light);
					}
				}

				glDepthMask(GL_TRUE);
				glDepthFunc(GL_LESS);
				glDisable(GL_BLEND);

		}
		clearTransparentQueue();
	}
	

	if (!captureMode && !PostProcessing::isActive()) {
		FrameBuffer::resetDefaultBuffer();
		_mainFrameBuffer->render();	
	}
}

void Renderer::renderAmbient(int pass, LightMap* lightmap, AABB* volume) {
	_skyBox.render();


	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it) {
		if ((*it)->getEnable() && Camera::getMainCamera()->frustumTest(*it)) {
			if (!(*it)->gameObject->getStatic()) continue;


			if (typeid((**it)) != typeid(MeshRenderer) || ((*it)->getMaterial() != nullptr && (*it)->getMaterial()->shader != nullptr)) {
				glEnable(GL_DEPTH_TEST);
				(*it)->render();
			}
			else {
				glEnable(GL_DEPTH_TEST);


				// IBL
				_ambientLightShader->bind();
				lightmap->irradianceMap->bind(_ambientLightShader->textures["irradianceMap"].unit);
				lightmap->specularMap->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);

				if(pass == 0){
					_ambientLightShader->setUniform("useCorrection", false);
				}else {
					_ambientLightShader->setUniform("minBound", volume->getBoundPosition());
					_ambientLightShader->setUniform("maxBound", volume->getMaxBoundGlobal());
					_ambientLightShader->setUniform("boundPos", volume->getBoundPosition());
					_ambientLightShader->setUniform("useCorrection", true);
				}

			
				// AMBIENT
				AmbientLight* ambient = Lighting::getLights<AmbientLight>()[0];
				(*it)->render(_ambientLightShader, ambient);


				if (pass == 0) {
					glEnable(GL_BLEND);
					glBlendFunc(GL_ONE, GL_ONE);
					glDepthMask(GL_FALSE);
					glDepthFunc(GL_EQUAL);

					std::unordered_map<std::type_index, std::vector<LightEmitter*>> lights = Lighting::getAllLights();
					for (auto const &lightvec : lights) {
						for (auto const &light : lightvec.second) {
							if (!light->getEnable() || (typeid(*(light->getLightShader())) == typeid(*(ForwardAmbientLightShader::getInstance())))) continue;
							(*it)->render(light->getLightShader(), light);
						}
					}

					glDepthMask(GL_TRUE);
					glDepthFunc(GL_LESS);
					glDisable(GL_BLEND);
				}
			}
		}
	}
}

void Renderer::render(Shader* customShader, bool captureMode) {
	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it) {
		if ((*it)->getEnable() && (!(!(*it)->captureble && captureMode))) {
			glEnable(GL_DEPTH_TEST);
			std::vector<DirectionalLight*> directionalLights = Lighting::getLights<DirectionalLight>();
			for (size_t i = 0; i < directionalLights.size(); ++i) {
				if (!directionalLights[i]->getEnable()) continue;
				(*it)->render(customShader, directionalLights[i]);
			}
		}
	}
}

void Renderer::renderGizmos() {
	if (_isDebug) {
		for (std::vector<GameObject*>::iterator it = SceneManager::getCurrentScene()->gameObjectList.begin(); it != SceneManager::getCurrentScene()->gameObjectList.end(); ++it) {
			if ((*it)->getEnable()) {
				(*it)->transform->renderHandels();
			}
		}
	}
}

void Renderer::updateTransparentQueue() {
	for (std::vector<ObjectRenderer*>::iterator it = _renderListTransparent.begin(); it != _renderListTransparent.end(); ++it) {
		if ((*it)->getEnable() && (*it)->gameObject->getStatic() && Camera::getMainCamera()->frustumTest(*it)) {
			//if (!(*it)->captureVisible && captureMode) continue;
			Vector3f camPos = Camera::getMainCamera()->transform->getPositionWorld();

			float dist = (*it)->transform->getPositionWorld().distanceSqrd(camPos);

			_renderQueueTransparent[dist] = *it;
		}
	}
}

void Renderer::clearTransparentQueue() {
	_renderQueueTransparent.clear();
}

void Renderer::setDebug(bool debug) {
	_isDebug = debug;
}

void Renderer::destroy() {
	_renderListOpaque.clear();
}

void Renderer::printList(){
	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it) {
		std::cout << (*it)->getName() << std::endl;
	}
}

void Renderer::setClearColor(Vector3f color){
	_clearColor = color;
}
