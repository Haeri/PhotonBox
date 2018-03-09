#include <algorithm>
#include <string>
#include <iostream>
#include "../../Components/AmbientLight.h"
#include "../../Components/Camera.h"
#include "../../Components/DirectionalLight.h"
#include "../../Components/MeshRenderer.h"
#include "../../Components/ObjectRenderer.h"
#include "../../Components/PointLight.h"
#include "../../Components/SpotLight.h"
#include "../../Components/Transform.h"
#include "../../Components/TransparentMeshRenderer.h"
#include "../../Resources/DepthShader.h"
#include "../../Resources/GShader.h"
#include "../../Resources/LitShader.h"
#include "../../Resources/Scene.h"
#include "../../Resources/TransparentShader.h"
#include "../../Resources/DeferredShader.cpp"
#include "../DeferredBuffer.h"
#include "../Display.h"
#include "../FrameBuffer.h"
#include "../PostProcessor.h"
#include "Lighting.h"
#include "Renderer.h"
#include "SceneManager.h"

DeferredBuffer Renderer::defBuffer;
bool Renderer::_isDebug;
SkyBox Renderer::_skyBox;
std::vector<ObjectRenderer*> Renderer::_renderListOpaque;
std::vector<ObjectRenderer*> Renderer::_renderListTransparent;
std::map<float, TransparentMeshRenderer*> Renderer::_renderQueueTransparent;
int Renderer::_drawCalls;

ForwardAmbientLightShader* Renderer::_ambientLightShader;
ForwardDirectionalLightShader* Renderer::_directionalLightShader;
ForwardPointLightShader* Renderer::_pointLightShader;
ForwardSpotLightShader* Renderer::_spotLightShader;
TransparentShader* Renderer::_transparentBaseShader;
GShader* Renderer::_gShader;
DeferredShader* Renderer::_deferredShader;
Material* Renderer::_deferredMaterial;

FrameBuffer* Renderer::_mainFrameBuffer;
FrameBuffer* Renderer::_debugFrameBuffer;
Vector3f Renderer::_clearColor = Vector3f(0.3, 0.3, 0.3);

void Renderer::addToRenderQueue(ObjectRenderer *objectRenderer, bool isOpaque)
{
	if (isOpaque)
	{
		_renderListOpaque.push_back(objectRenderer);
	}
	else
	{
		_renderListTransparent.push_back(objectRenderer);
	}
}

void Renderer::removeFromRenderQueue(ObjectRenderer *objectRenderer)
{
	if (objectRenderer->isOpaque())
	{
		_renderListOpaque.erase(std::remove(_renderListOpaque.begin(), _renderListOpaque.end(), objectRenderer), _renderListOpaque.end());
	}
	else
	{
		_renderListTransparent.erase(std::remove(_renderListTransparent.begin(), _renderListTransparent.end(), objectRenderer), _renderListTransparent.end());
	}
}

void Renderer::setSkyBox(CubeMap* cubeMap)
{
	_skyBox.setLightProbe(new LightProbe(cubeMap));
}

void Renderer::init()
{
	init(1);
}

void Renderer::init(float superSampling)
{
	// OpenGL config
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glClearColor(_clearColor.x(), _clearColor.y(), _clearColor.z(), 0);

	_ambientLightShader = ForwardAmbientLightShader::getInstance();
	_directionalLightShader = ForwardDirectionalLightShader::getInstance();
	_pointLightShader = ForwardPointLightShader::getInstance();
	_spotLightShader = ForwardSpotLightShader::getInstance();
	_transparentBaseShader = TransparentShader::getInstance();
	_gShader = GShader::getInstance();
	_deferredShader = DeferredShader::getInstance();
	_mainFrameBuffer = new FrameBuffer(superSampling);
	_mainFrameBuffer->addTextureAttachment("color", true, false);
	_mainFrameBuffer->addDepthBufferAttachment();
	_mainFrameBuffer->ready();
	_debugFrameBuffer = new FrameBuffer(1);
	_debugFrameBuffer->addTextureAttachment("color", false, false);
	_debugFrameBuffer->ready();
	defBuffer.init();
	_isDebug = false;
	_deferredMaterial = new Material(_deferredShader);
	_deferredMaterial->setTexture("gPosition", defBuffer.gBuffer, "gPosition");
	_deferredMaterial->setTexture("gNormal", defBuffer.gBuffer, "gNormal");
	_deferredMaterial->setTexture("gRoughness", defBuffer.gBuffer, "gRoughness");
	_deferredMaterial->setTexture("gMetallic", defBuffer.gBuffer, "gMetallic");
	_deferredMaterial->setTexture("gAlbedo", defBuffer.gBuffer, "gAlbedo");
}

void Renderer::start()
{
	_skyBox.init();

	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
	{
		(*it)->init();
	}
	for (std::vector<ObjectRenderer*>::iterator it = _renderListTransparent.begin(); it != _renderListTransparent.end(); ++it)
	{
		(*it)->init();
	}
}

void Renderer::prePass()
{
	defBuffer.gBuffer->enable();
	defBuffer.gBuffer->clear();

	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
	{
		if ((*it)->getEnable())
		{
			glEnable(GL_DEPTH_TEST);
			(*it)->render(_gShader);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::clearDrawCalls()
{
	_drawCalls = 0;
}

void Renderer::render()
{
	render(false);
}

void Renderer::renderShadows(bool captureMode)
{
	glCullFace(GL_FRONT);
	std::vector<DirectionalLight*> directionalLights = Lighting::getLights<DirectionalLight>();
	for (size_t i = 0; i < directionalLights.size(); ++i)
	{
		if (!directionalLights[i]->getEnable()) continue;
		directionalLights[i]->renderShadowMap(captureMode);
	}
	glCullFace(GL_BACK);
}

void Renderer::renderDeferred() {
	_mainFrameBuffer->enable();
	_mainFrameBuffer->clear();
	
	_skyBox.render();

	std::unordered_map<std::type_index, std::vector<LightEmitter*>> lights = Lighting::getAllLights();

	_deferredShader->bind();

	int i = -1;
	for (auto const &lightvec : lights)
	{

		for (auto const &light : lightvec.second)
		{
			if (!light->getEnable() || (typeid(*(light->getLightShader())) == typeid(*(ForwardAmbientLightShader::getInstance())))) continue;

			if ((typeid(*(light->getLightShader())) == typeid(*(ForwardPointLightShader::getInstance()))))
			{
				Vector3f lightViewPos = (Camera::getMainCamera()->getViewMatrix() * Vector4f(light->transform->getPositionWorld(), 1.0f)).xyz();
				++i;
				_deferredShader->setUniform("lights[" + std::to_string(i) + "].Position", lightViewPos);
				_deferredShader->setUniform("lights[" + std::to_string(i) + "].Color", light->color);
				_deferredShader->setUniform("lights[" + std::to_string(i) + "].Linear", 0.7f);
				_deferredShader->setUniform("lights[" + std::to_string(i) + "].Quadratic", 1.8f);
			}
		}
	}
	

	defBuffer.gBuffer->render(_deferredMaterial);

	/*
	int cols = 2;
	int widthX = 0;

	glViewport(widthX, 0, Display::getWidth() / cols, Display::getHeight() / cols);
	//defBuffer.gBuffer->render("gAlbedo");
	defBuffer.gBuffer->render(_deferredMaterial);

	widthX += Display::getWidth() / cols;
	*/
}

void Renderer::render(bool captureMode)
{
	render(captureMode, nullptr);
}

void Renderer::render(bool captureMode, LightMap* lightmap)
{
	if (!captureMode)
	{
		// Bind & clear Shadow FBO
		renderShadows(false);

		// Bind & clear Main FBO
		//_mainFrameBuffer->enable();
		//_mainFrameBuffer->clear();
	}

	//_skyBox.render();

	/*
	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
	{
		if ((*it)->getEnable() && Camera::getMainCamera()->frustumTest(*it))
		{
			if (!(*it)->captureble && captureMode) continue;


			if (typeid((**it)) != typeid(MeshRenderer) || ((*it)->getMaterial() != nullptr && (*it)->getMaterial()->shader != nullptr))
			{
				glEnable(GL_DEPTH_TEST);
				(*it)->render();
			}
			else
			{
				glEnable(GL_DEPTH_TEST);


				// IBL
				_ambientLightShader->bind();
				LightProbe* lp = Lighting::findInLightProberVolume((*it)->transform);
				if (!captureMode && lp != nullptr)
				{
					lp->getIrradianceCube()->bind(_ambientLightShader->textures["irradianceMap"].unit);
					lp->getSpecularCube()->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);

					_ambientLightShader->setUniform("minBound", lp->bounds.getMinBoundGlobal());
					_ambientLightShader->setUniform("maxBound", lp->bounds.getMaxBoundGlobal());
					_ambientLightShader->setUniform("boundPos", lp->bounds.getBoundPosition());
					_ambientLightShader->setUniform("useCorrection", true);
				}
				else if (lightmap != nullptr)
				{
					lightmap->irradianceMap->bind(_ambientLightShader->textures["irradianceMap"].unit);
					lightmap->specularMap->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);
					_ambientLightShader->setUniform("useCorrection", false);
				}
				else
				{
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
				for (auto const &lightvec : lights)
				{
					for (auto const &light : lightvec.second)
					{
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
	*/

	if (!captureMode)
	{
		updateTransparentQueue();
		for (std::map<float, TransparentMeshRenderer*>::reverse_iterator it = _renderQueueTransparent.rbegin(); it != _renderQueueTransparent.rend(); ++it)
		{
			glEnable(GL_DEPTH_TEST);

			// IBL
			_transparentBaseShader->bind();
			LightProbe* lp = Lighting::findInLightProberVolume(it->second->transform);
			if (!captureMode && lp != nullptr)
			{
				lp->getSpecularCube()->bind(_transparentBaseShader->textures["convolutedSpecularMap"].unit);

				_transparentBaseShader->setUniform("minBound", lp->bounds.getMinBoundGlobal());
				_transparentBaseShader->setUniform("maxBound", lp->bounds.getMaxBoundGlobal());
				_transparentBaseShader->setUniform("boundPos", lp->bounds.getBoundPosition());
				_transparentBaseShader->setUniform("useCorrection", true);
			}
			else
			{
				_skyBox.getLightProbe()->getSpecularCube()->bind(_transparentBaseShader->textures["convolutedSpecularMap"].unit);
				_transparentBaseShader->setUniform("useCorrection", false);
			}


			// AMBIENT
			AmbientLight* ambient = Lighting::getLights<AmbientLight>()[0];

			glEnable(GL_BLEND);
			if (it->second->cutout)
			{
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			else
			{
				glBlendFunc(GL_ONE, GL_ONE);
			}

			it->second->render(_transparentBaseShader, ambient);


			glBlendFunc(GL_ONE, GL_ONE);
			glDepthMask(GL_FALSE);
			glDepthFunc(GL_EQUAL);

			std::unordered_map<std::type_index, std::vector<LightEmitter*>> lights = Lighting::getAllLights();
			for (auto const &lightvec : lights)
			{
				for (auto const &light : lightvec.second)
				{
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


	if (!captureMode && !PostProcessing::isActive())
	{
		FrameBuffer::resetDefaultBuffer();
		_mainFrameBuffer->render("color");
	}
}

void Renderer::renderAmbient(int pass, LightMap* lightmap, AABB* volume)
{
	_skyBox.render();

	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
	{
		if ((*it)->getEnable() && Camera::getMainCamera()->frustumTest(*it))
		{
			if (!(*it)->gameObject->getStatic()) continue;


			if (typeid((**it)) != typeid(MeshRenderer) || ((*it)->getMaterial() != nullptr && (*it)->getMaterial()->shader != nullptr))
			{
				glEnable(GL_DEPTH_TEST);
				(*it)->render();
			}
			else
			{
				glEnable(GL_DEPTH_TEST);


				// IBL
				_ambientLightShader->bind();
				lightmap->irradianceMap->bind(_ambientLightShader->textures["irradianceMap"].unit);
				lightmap->specularMap->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);

				if (pass == 0)
				{
					_ambientLightShader->setUniform("useCorrection", false);
				}
				else
				{
					_ambientLightShader->setUniform("minBound", volume->getBoundPosition());
					_ambientLightShader->setUniform("maxBound", volume->getMaxBoundGlobal());
					_ambientLightShader->setUniform("boundPos", volume->getBoundPosition());
					_ambientLightShader->setUniform("useCorrection", true);
				}


				// AMBIENT
				AmbientLight* ambient = Lighting::getLights<AmbientLight>()[0];
				(*it)->render(_ambientLightShader, ambient);


				if (pass == 0)
				{
					glEnable(GL_BLEND);
					glBlendFunc(GL_ONE, GL_ONE);
					glDepthMask(GL_FALSE);
					glDepthFunc(GL_EQUAL);

					std::unordered_map<std::type_index, std::vector<LightEmitter*>> lights = Lighting::getAllLights();
					for (auto const &lightvec : lights)
					{
						for (auto const &light : lightvec.second)
						{
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

void Renderer::render(Shader* customShader, bool captureMode)
{
	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
	{
		if ((*it)->getEnable() && (!(!(*it)->captureble && captureMode)))
		{
			glEnable(GL_DEPTH_TEST);
			std::vector<DirectionalLight*> directionalLights = Lighting::getLights<DirectionalLight>();
			for (size_t i = 0; i < directionalLights.size(); ++i)
			{
				if (!directionalLights[i]->getEnable()) continue;
				(*it)->render(customShader, directionalLights[i]);
			}
		}
	}
}

void Renderer::renderGizmos()
{
	if (_isDebug)
	{
		for (std::vector<GameObject*>::iterator it = SceneManager::getCurrentScene()->gameObjectList.begin(); it != SceneManager::getCurrentScene()->gameObjectList.end(); ++it)
		{
			if ((*it)->getEnable())
			{
				(*it)->transform->renderHandels();
			}
		}
	}

	/*
	int cols = 4;
	int widthX = 0;

	glViewport(widthX, 0, Display::getWidth() / cols, Display::getHeight() / cols);
	defBuffer.gBuffer->render("gAlbedo");
	widthX += Display::getWidth() / cols;

	glViewport(widthX, 0, Display::getWidth() / cols, Display::getHeight() / cols);
	defBuffer.gBuffer->render("gPosition");
	widthX += Display::getWidth() / cols;

	glViewport(widthX, 0, Display::getWidth() / cols, Display::getHeight() / cols);
	defBuffer.gBuffer->render("gNormal");
	widthX += Display::getWidth() / cols;

	glViewport(widthX, 0, Display::getWidth() / cols, Display::getHeight() / cols);
	defBuffer.gBuffer->render("gMetallic");
	widthX += Display::getWidth() / cols;

	
	glViewport(widthX, 0, Display::getWidth() / cols, Display::getHeight() / cols);
	defBuffer.gBuffer->render("gRoughness");
	widthX += Display::getWidth() / cols;
	*/

	FrameBuffer::resetDefaultBuffer();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_debugFrameBuffer->render("color");

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	_debugFrameBuffer->enable();
	_debugFrameBuffer->clear();
	
	
}

void Renderer::updateTransparentQueue()
{
	float bias = 0.0001f;
	for (std::vector<ObjectRenderer*>::iterator it = _renderListTransparent.begin(); it != _renderListTransparent.end(); ++it)
	{
		if ((*it)->getEnable() && (*it)->gameObject->getStatic() && Camera::getMainCamera()->frustumTest(*it))
		{
			//if (!(*it)->captureVisible && captureMode) continue;
			Vector3f camPos = Camera::getMainCamera()->transform->getPositionWorld();

			float dist = (*it)->transform->getPositionWorld().distanceSqrd(camPos);

			while (1)
			{
				if (_renderQueueTransparent.find(dist) == _renderQueueTransparent.end())
					break;
				else
					dist += bias;
			}

			_renderQueueTransparent[dist] = dynamic_cast<TransparentMeshRenderer*> (*it);
		}
	}
}

void Renderer::clearTransparentQueue()
{
	_renderQueueTransparent.clear();
}

void Renderer::setDebug(bool debug)
{
	_isDebug = debug;
}

void Renderer::destroy()
{
	_renderListOpaque.clear();
}

void Renderer::printList()
{
	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
	{
		std::cout << (*it)->getName() << std::endl;
	}
}

void Renderer::setClearColor(Vector3f color)
{
	_clearColor = color;
}

void Renderer::addDrawCall()
{
	++_drawCalls;
}
