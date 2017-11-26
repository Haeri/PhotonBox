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
#include "../../Core/FrameBuffer.h"
#include "../PostProcessor.h"
#include "../../Resources/LitShader.h"
#include "../../Resources/DepthShader.h"

bool Renderer::_isDebug;
SkyBox Renderer::_skyBox;
std::vector<ObjectRenderer*> Renderer::_renderQueueOpaque;
std::map<ObjectRenderer*, float> Renderer::_renderQueueTransparent;

ForwardAmbientLightShader* Renderer::_ambientLightShader;
ForwardDirectionalLightShader* Renderer::_directionalLightShader;
ForwardPointLightShader* Renderer::_pointLightShader;
FrameBuffer* Renderer::_mainFrameBuffer;
Vector3f Renderer::_clearColor = Vector3f(0.3, 0.3, 0.3);

void Renderer::addToRenderQueue(ObjectRenderer *objectRenderer, bool isOpaque) {
	if (isOpaque) {
		_renderQueueOpaque.push_back(objectRenderer);
	}else {
		_renderQueueTransparent.insert(std::pair<ObjectRenderer*, float>(objectRenderer, -1));
	}
}

void Renderer::removeFromRenderQueue(ObjectRenderer *objectRenderer) {
	if (objectRenderer->isOpaque()) {
		_renderQueueOpaque.erase(std::remove(_renderQueueOpaque.begin(), _renderQueueOpaque.end(), objectRenderer), _renderQueueOpaque.end());
	}else {
		// TODO: DO this
		//_renderQueueTransparent.erase(std::remove(_renderQueueTransparent.begin(), _renderQueueTransparent.end(), objectRenderer), _renderQueueTransparent.end());
	}	
}

void Renderer::setSkyBox(CubeMap* cubeMap){
	_skyBox.setLightProbe(new LightProbe(cubeMap));
}

void Renderer::init() {
	// OpenGL config
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glClearColor(_clearColor.x(), _clearColor.y(), _clearColor.z(), 1);
	
	_ambientLightShader = ForwardAmbientLightShader::getInstance();
	_directionalLightShader = ForwardDirectionalLightShader::getInstance();
	_pointLightShader = ForwardPointLightShader::getInstance();
	_mainFrameBuffer = new FrameBuffer(Display::getWidth(), Display::getHeight());

	_isDebug = false;
}

void Renderer::start() {
	_skyBox.init();

	for (std::vector<ObjectRenderer*>::iterator it = _renderQueueOpaque.begin(); it != _renderQueueOpaque.end(); ++it) {
		(*it)->init();
	}
}

void Renderer::render() {
	render(false);
}

void Renderer::renderShadows() {
	glCullFace(GL_FRONT);
	std::vector<DirectionalLight*> directionalLights = Lighting::getLights<DirectionalLight>();
	for (size_t i = 0; i < directionalLights.size(); ++i) {
		if (!directionalLights[i]->getEnable()) continue;
		directionalLights[i]->renderShadowMap();
	}
	glCullFace(GL_BACK);
}

void Renderer::render(bool captureMode) {
	if (!captureMode) {
		renderShadows();
		_mainFrameBuffer->enable();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	_skyBox.render();

	
	for (std::vector<ObjectRenderer*>::iterator it = _renderQueueOpaque.begin(); it != _renderQueueOpaque.end(); ++it) {
		if ((*it)->getEnable()) {
			if (typeid((**it)) != typeid(MeshRenderer) || ((*it)->getMaterial() != nullptr && (*it)->getMaterial()->shader != nullptr)) {
				glEnable(GL_DEPTH_TEST);
				(*it)->render();
			}
			else {
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
				}
				else {
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


				/*
				std::unordered_map<std::type_index, std::vector<LightEmitter*>> lights = Lighting::getAllLights();
				for (auto const &lightvec : lights) {
					for (auto const &light : lightvec.second) {
						if (!light->getEnable()) continue;
						(*it)->render(light->getLightShader(), light);
					}
				}
				*/

				// DIRECTIONAL LIGHTS
				std::vector<DirectionalLight*> directionalLights = Lighting::getLights<DirectionalLight>();
				for (size_t i = 0; i < directionalLights.size(); ++i) {
					if (!directionalLights[i]->getEnable()) continue;
					(*it)->render(_directionalLightShader, directionalLights[i]);
				}

				// POINT LIGHTS
				std::vector<PointLight*> pointLights = Lighting::getLights<PointLight>();
				for (size_t i = 0; i < pointLights.size(); ++i) {
					if (!pointLights[i]->getEnable()) continue;
					(*it)->render(_pointLightShader, pointLights[i]);
				}

				// SPOT LIGHTS

				glDepthMask(GL_TRUE);
				glDepthFunc(GL_LESS);
				glDisable(GL_BLEND);

			}
		}
	}
	

	sortTransparents();
	for (std::map<ObjectRenderer*, float>::iterator it = _renderQueueTransparent.begin(); it != _renderQueueTransparent.end(); ++it) {
		glEnable(GL_DEPTH_TEST);

		/*
		// IBL
		_ambientLightShader->bind();
		LightProbe* lp = Lighting::findInLightProberVolume(it->first->transform);
		if (!captureMode && lp != nullptr) {
		lp->getIrradianceCube()->bind(_ambientLightShader->textures["irradianceMap"].unit);
		lp->getSpecularCube()->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);

		_ambientLightShader->setUniform("minBound", lp->bounds.getMinBoundGlobal());
		_ambientLightShader->setUniform("maxBound", lp->bounds.getMaxBoundGlobal());
		_ambientLightShader->setUniform("boundPos", lp->bounds.getBoundPosition());
		_ambientLightShader->setUniform("useCorrection", true);
		}
		else {
		_skyBox.getLightProbe()->getIrradianceCube()->bind(_ambientLightShader->textures["irradianceMap"].unit);
		_skyBox.getLightProbe()->getSpecularCube()->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);
		_ambientLightShader->setUniform("useCorrection", false);
		}
		*/

		// AMBIENT
	//	AmbientLight* ambient = Lighting::getLights<AmbientLight>()[0];
		it->first->render(LitShader::getInstance());

//		AmbientLight* ambient = Lighting::getLights<AmbientLight>()[0];
//		it->first->render(_ambientLightShader, ambient);


		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);



		// DIRECTIONAL LIGHTS
		std::vector<DirectionalLight*> directionalLights = Lighting::getLights<DirectionalLight>();
		for (size_t i = 0; i < directionalLights.size(); ++i) {
			if (!directionalLights[i]->getEnable()) continue;
			it->first->render(_directionalLightShader, directionalLights[i]);
		}

		// POINT LIGHTS
		std::vector<PointLight*> pointLights = Lighting::getLights<PointLight>();
		for (size_t i = 0; i < pointLights.size(); ++i) {
			if (!pointLights[i]->getEnable()) continue;
			it->first->render(_pointLightShader, pointLights[i]);
		}

		// SPOT LIGHTS

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);

	}
	
	
	if(_isDebug){
		for (std::vector<ObjectRenderer*>::iterator it = _renderQueueOpaque.begin(); it != _renderQueueOpaque.end(); ++it) {
			if ((*it)->getEnable()) {
				(*it)->transform->renderHandels();
			}
		}
	}

	if (!captureMode && !PostProcessing::isActive()) {
		FrameBuffer::resetDefaultBuffer();
		_mainFrameBuffer->render();	
	}
}

void Renderer::render(Shader* customShader) {
	for (std::vector<ObjectRenderer*>::iterator it = _renderQueueOpaque.begin(); it != _renderQueueOpaque.end(); ++it) {
		if ((*it)->getEnable()) {
			glEnable(GL_DEPTH_TEST);
			std::vector<DirectionalLight*> directionalLights = Lighting::getLights<DirectionalLight>();
			for (size_t i = 0; i < directionalLights.size(); ++i) {
				if (!directionalLights[i]->getEnable()) continue;
				(*it)->render(customShader, directionalLights[i]);
			}
		}
	}
}

void Renderer::sortTransparents() {
	Vector3f camPos = Camera::getMainCamera()->transform->getPositionWorld();
	std::map<ObjectRenderer*, float>::iterator it;
	for (it = _renderQueueTransparent.begin(); it != _renderQueueTransparent.end(); it++) {
		float dist = it->first->transform->getPositionWorld().distanceSqrd(camPos);
		it->second = dist;
	}
}

void Renderer::setDebug(bool debug) {
	_isDebug = debug;
}

void Renderer::destroy() {
	_renderQueueOpaque.clear();
}

void Renderer::printList(){
	for (std::vector<ObjectRenderer*>::iterator it = _renderQueueOpaque.begin(); it != _renderQueueOpaque.end(); ++it) {
		std::cout << (*it)->getName() << std::endl;
	}
}

void Renderer::setClearColor(Vector3f color){
	_clearColor = color;
}
