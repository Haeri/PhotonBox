#include <algorithm>
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

bool Renderer::_isDebug;
SkyBox Renderer::_skyBox;
std::vector<ObjectRenderer*> Renderer::_renderQueue;
Renderer::RenderMode Renderer::renderMode;

void Renderer::addToRenderQueue(ObjectRenderer *objectRenderer) {
	_renderQueue.push_back(objectRenderer);
}

void Renderer::removeFromRenderQueue(ObjectRenderer *objectRenderer) {
	_renderQueue.erase(std::remove(_renderQueue.begin(), _renderQueue.end(), objectRenderer), _renderQueue.end());
}

void Renderer::setSkyBox(CubeMap* cubeMap){
	_skyBox.setCubeMap(cubeMap);
}

void Renderer::init(RenderMode mode) {
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	
	renderMode = mode;
	_isDebug = false;
}

void Renderer::start() {
	_skyBox.init();

	for (std::vector<ObjectRenderer*>::iterator it = _renderQueue.begin(); it != _renderQueue.end(); ++it) {
		(*it)->init();
	}
}

void Renderer::render() {
	Display::clearDisplay(0.1, 0.1, 0.1, 1);

	_skyBox.render();

	for (std::vector<ObjectRenderer*>::iterator it = _renderQueue.begin(); it != _renderQueue.end(); ++it) {
		if ((*it)->getEnable()) {
			if (Renderer::renderMode == Renderer::RenderMode::CUSTOM) {
				(*it)->render();
			}else if (Renderer::renderMode == Renderer::RenderMode::FORWARD) {



				

				glBindVertexArray(_vao);


				// AMBIENT
				AmbientLight* ambient = Lighting::getLights<AmbientLight>()[0];
				_material->forwardShader->bindAmbientShader();
				_material->forwardShader->ambientLightShader->update(mvp, modelMatrix, *ambient, eyePos);
				_material->forwardShader->ambientLightShader->enableAttributes();

				_material->updateUniforms();
				_material->updateTextures();

				if (Renderer::getSkyBox()->getCubeMap() != nullptr) {
					Renderer::getSkyBox()->getCubeMap()->bind(_material->forwardShader->ambientLightShader->textures["skyBoxLod0"].unit, 0);
					Renderer::getSkyBox()->getCubeMap()->bind(_material->forwardShader->ambientLightShader->textures["skyBoxLod1"].unit, 1);
					Renderer::getSkyBox()->getCubeMap()->bind(_material->forwardShader->ambientLightShader->textures["skyBoxLod2"].unit, 2);
					Renderer::getSkyBox()->getCubeMap()->bind(_material->forwardShader->ambientLightShader->textures["skyBoxLod3"].unit, 3);
				}



				_material->forwardShader->ambientLightShader->updateTextures();

				glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
				_material->forwardShader->ambientLightShader->disableAttributes();



				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				glDepthMask(GL_FALSE);
				glDepthFunc(GL_EQUAL);


				// DIRECTIONAL LIGHTS
				std::vector<DirectionalLight*> directionalLights = Lighting::getLights<DirectionalLight>();
				for (size_t i = 0; i < directionalLights.size(); ++i) {
					if (!directionalLights[i]->getEnable()) continue;
					_material->forwardShader->bindDirectionalLightShader();

					_material->forwardShader->directionalLightShader->update(mvp, modelMatrix, *directionalLights[i], eyePos);
					_material->forwardShader->directionalLightShader->enableAttributes();


					if (_material->albedoMap != nullptr) _material->albedoMap->bind(_material->forwardShader->directionalLightShader->textures["albedoMap"].unit);
					else default_specular->bind(_material->forwardShader->directionalLightShader->textures["albedoMap"].unit);

					if (_material->normalMap != nullptr) _material->normalMap->bind(_material->forwardShader->directionalLightShader->textures["normalMap"].unit);
					else default_normal->bind(_material->forwardShader->directionalLightShader->textures["normalMap"].unit);

					if (_material->specularMap != nullptr) _material->specularMap->bind(_material->forwardShader->directionalLightShader->textures["specularMap"].unit);
					else default_specular->bind(_material->forwardShader->directionalLightShader->textures["specularMap"].unit);

					_material->forwardShader->directionalLightShader->updateTextures();

					glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
					_material->forwardShader->directionalLightShader->disableAttributes();
				}

				// POINT LIGHTS
				std::vector<PointLight*> pointLights = Lighting::getLights<PointLight>();
				for (size_t i = 0; i < pointLights.size(); ++i) {
					if (!pointLights[i]->getEnable()) continue;

					(*it)->render(_material->forwardShader->pointLightShader);

				
					_material->forwardShader->pointLightShader->update(mvp, modelMatrix, *pointLights[i], eyePos);
				
				}

				// SPOT LIGHTS


				glDepthMask(GL_TRUE);
				glDepthFunc(GL_LESS);
				glDisable(GL_BLEND);


				glBindVertexArray(0);

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
