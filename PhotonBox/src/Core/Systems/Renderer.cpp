#include <algorithm>
#include <string>
#include <iostream>
#include "PhotonBox/core/systems/Renderer.h"

#include "PhotonBox/components/AmbientLight.h"
#include "PhotonBox/components/Camera.h"
#include "PhotonBox/components/DirectionalLight.h"
#include "PhotonBox/components/ObjectRenderer.h"
#include "PhotonBox/components/MeshRenderer.h"
#include "PhotonBox/components/PointLight.h"
#include "PhotonBox/components/SpotLight.h"
#include "PhotonBox/components/Transform.h"
#include "PhotonBox/components/TransparentMeshRenderer.h"
#include "PhotonBox/resources/DepthShader.h"
#include "PhotonBox/resources/GShader.h"
#include "PhotonBox/resources/LitShader.h"
#include "PhotonBox/resources/Scene.h"
#include "PhotonBox/resources/TransparentShader.h"
#include "PhotonBox/resources/DeferredShader.h"
#include "PhotonBox/core/Display.h"
#include "PhotonBox/core/FrameBuffer.h"
#include "PhotonBox/core/PostProcessor.h"
#include "PhotonBox/core/systems/Lighting.h"
#include "PhotonBox/core/systems/SceneManager.h"
#include "PhotonBox/core/Entity.h"
#include "PhotonBox/resources/CircleShader.h"
#include "PhotonBox/components/LightProbe.h"
#include "PhotonBox/core/LightMap.h"

#include "imgui/imgui.h"

int Renderer::_debugMode;
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
DeferredShader* Renderer::_deferredShader;
Material* Renderer::_deferredMaterial;

FrameBuffer* Renderer::_mainFrameBuffer;
FrameBuffer* Renderer::_gBuffer;
FrameBuffer* Renderer::_gizmoBuffer;
Vector3f Renderer::_clearColor = Vector3f(0.3f, 0.3f, 0.3f);

// Temp stuff
GLuint _quadVAO = -1;
GLuint _quadVBO;
std::vector<float> stuff;


float factor = 0.2f;
std::string buffers[8] =
{
	"gPosition",
	"gNormal",
	"gMetallic",
	"gRoughness",
	"gAlbedo",
	"gEmission",
	"gIrradiance",
	"gRadiance",
};


void Renderer::addToRenderQueue(ObjectRenderer * renderer, RenderType type)
{
	if (type == RenderType::transparent)
	{
		_renderListTransparent.push_back(renderer);
	}
	else
	{
		_renderListOpaque.push_back(renderer);
	}
}

void Renderer::removeFromRenderQueue(ObjectRenderer *renderer)
{
	if (renderer->getType() == RenderType::transparent)
	{
		_renderListTransparent.erase(std::remove(_renderListTransparent.begin(), _renderListTransparent.end(), renderer), _renderListTransparent.end());
	}
	else
	{
		_renderListOpaque.erase(std::remove(_renderListOpaque.begin(), _renderListOpaque.end(), renderer), _renderListOpaque.end());
	}
}

void Renderer::setSkyBox(CubeMap* cubeMap)
{
	_skyBox.setCubeMap(*cubeMap);
	_skyBox.setDrawMode(SkyBox::DRAW_CUBE_MAP);
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
	_deferredShader = DeferredShader::getInstance();
	
	_mainFrameBuffer = new FrameBuffer(superSampling);
	_mainFrameBuffer->addTextureAttachment("color", true, false);
	_mainFrameBuffer->addDepthBufferAttachment();
	_mainFrameBuffer->ready();

	_gBuffer = new FrameBuffer(superSampling);
	_gBuffer->addTextureAttachment("gPosition", true);
	_gBuffer->addTextureAttachment("gNormal", true);
	_gBuffer->addTextureAttachment("gMetallic");
	_gBuffer->addTextureAttachment("gRoughness");
	_gBuffer->addTextureAttachment("gAlbedo");
	_gBuffer->addTextureAttachment("gEmission", true);
	//_gBuffer->addTextureAttachment("gAO");
	_gBuffer->addTextureAttachment("gIrradiance", true);
	_gBuffer->addTextureAttachment("gRadiance", true);
	//_gBuffer->addDepthTextureAttachment("gDepth");
	_gBuffer->addDepthBufferAttachment();
	_gBuffer->ready();

	_gizmoBuffer = new FrameBuffer(1);
	_gizmoBuffer->addTextureAttachment("color", false, false);
	_gizmoBuffer->ready();
	
	_deferredMaterial = new Material(_deferredShader);
	_deferredMaterial->setTexture("gPosition", _gBuffer, "gPosition");
	_deferredMaterial->setTexture("gNormal", _gBuffer, "gNormal");
	_deferredMaterial->setTexture("gRoughness", _gBuffer, "gRoughness");
	_deferredMaterial->setTexture("gMetallic", _gBuffer, "gMetallic");
	_deferredMaterial->setTexture("gAlbedo", _gBuffer, "gAlbedo");
	_deferredMaterial->setTexture("gIrradiance", _gBuffer, "gIrradiance");
	_deferredMaterial->setTexture("gRadiance", _gBuffer, "gRadiance");
	
	_debugMode = 0;
	
	float max = 100;
	int half = max / 2.0f;
	for (float x = 0; x < max; x += 1)
	{
		float y = 0;
	//	for (float y = 0; y < max; y += 10)
	//	{
			for (float z = 0; z < max; z += 1)
			{
				stuff.push_back(x- half);
				stuff.push_back(y);
				stuff.push_back(z- half);

				stuff.push_back(1- x/max);
				stuff.push_back(1- z/max);
				stuff.push_back(1- 1.0f/max);
			}
	//	}
	}
}

void Renderer::reset()
{
	_skyBox.reset();
	/*
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

	_gBuffer = new FrameBuffer(1);
	_gBuffer->addTextureAttachment("gPosition", true);
	_gBuffer->addTextureAttachment("gNormal", true);
	_gBuffer->addTextureAttachment("gMetallic");
	_gBuffer->addTextureAttachment("gRoughness");
	_gBuffer->addTextureAttachment("gAlbedo");
	_gBuffer->addTextureAttachment("gEmission", true);
	//_gBuffer->addTextureAttachment("gAO");
	_gBuffer->addTextureAttachment("gIrradiance", true);
	_gBuffer->addTextureAttachment("gRadiance", true);
	//_gBuffer->addDepthTextureAttachment("gDepth");
	_gBuffer->addDepthBufferAttachment();
	_gBuffer->ready();

	_gizmoBuffer = new FrameBuffer(1);
	_gizmoBuffer->addTextureAttachment("color", false, false);
	_gizmoBuffer->ready();

	_deferredMaterial = new Material(_deferredShader);
	_deferredMaterial->setTexture("gPosition", _gBuffer, "gPosition");
	_deferredMaterial->setTexture("gNormal", _gBuffer, "gNormal");
	_deferredMaterial->setTexture("gRoughness", _gBuffer, "gRoughness");
	_deferredMaterial->setTexture("gMetallic", _gBuffer, "gMetallic");
	_deferredMaterial->setTexture("gAlbedo", _gBuffer, "gAlbedo");
	_deferredMaterial->setTexture("gIrradiance", _gBuffer, "gIrradiance");
	_deferredMaterial->setTexture("gRadiance", _gBuffer, "gRadiance");

	_debugMode = 0;
	*/
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
	_gBuffer->enable();
	_gBuffer->clear();

	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
	{
		if ((*it)->getEnable() && Camera::getMainCamera()->frustumTest(*it))
		{
			glEnable(GL_DEPTH_TEST);

			if ((*it)->getType() == RenderType::cutout)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			Shader* shader = (*it)->getMaterial()->shader;

			if(typeid(*shader) == typeid(GShader))
			{
				// IBL
				shader->bind();
				LightProbe* lp = Lighting::findInLightProberVolume((*it)->transform);
				if (lp != nullptr)
				{
					lp->getIrradianceCube()->bind(shader->textures["irradianceMap"].unit);
					lp->getSpecularCube()->bind(shader->textures["convolutedSpecularMap"].unit);

					shader->setUniform("minBound", lp->bounds.getMinBoundGlobal());
					shader->setUniform("maxBound", lp->bounds.getMaxBoundGlobal());
					shader->setUniform("boundPos", lp->bounds.getBoundPosition());
					shader->setUniform("useCorrection", true);
				}
				else
				{
					_skyBox.getLightMap()->irradianceMap->bind(shader->textures["irradianceMap"].unit);
					_skyBox.getLightMap()->specularMap->bind(shader->textures["convolutedSpecularMap"].unit);
					shader->setUniform("useCorrection", false);
				}
			}
		
			(*it)->render();

			if ((*it)->getType() == RenderType::cutout)
			{
				glDisable(GL_BLEND);
			}
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
	
	// Render Shadows
	renderShadows(false);

	// Clear Buffer
	_mainFrameBuffer->enable();
	_mainFrameBuffer->clear();


	// Send light data to shader
	std::unordered_map<std::type_index, std::vector<LightEmitter*>> lights = Lighting::getAllLights();
	

	_deferredShader->bind();
	_deferredShader->setUniform<int>("numDirectionalLights", lights[typeid(DirectionalLight)].size());
	_deferredShader->setUniform<int>("numPointLights", lights[typeid(PointLight)].size());

	for (auto const &lightvec : lights)
	{
		int i = -1;
		for (auto const &light : lightvec.second)
		{
			if (!light->getEnable() || (typeid(*(light->getLightShader())) == typeid(*(ForwardAmbientLightShader::getInstance())))) continue;

			if (typeid(*light) == typeid(DirectionalLight))
			{
				DirectionalLight* dl = dynamic_cast<DirectionalLight*>(light);
				Vector3f lightViewDirection = (Camera::getMainCamera()->getViewMatrix() * Vector4f(dl->direction, 0.0f)).xyz();

				Matrix4f lightView = Matrix4f::lookAt(
					(dl->direction * -3),
					Vector3f(0.0f, 1.0f, 0.0f),
					dl->direction);
				Matrix4f lightSpaceMatrix = dl->lightProjection * lightView;

				++i;
				_deferredShader->setUniform("directionalLights[" + std::to_string(i) + "].lightSpaceMatrix", lightSpaceMatrix);
				_deferredShader->setUniform("directionalLights[" + std::to_string(i) + "].direction", lightViewDirection);
				_deferredShader->setUniform("directionalLights[" + std::to_string(i) + "].color", dl->color);
				_deferredShader->setUniform("directionalLights[" + std::to_string(i) + "].intensity", dl->intensity);

				dl->shadowBuffer->bind(_deferredShader->textures["shadowMap"].unit, "depth");
			}
			else if (typeid(*light) == typeid(PointLight))
			{
				PointLight* dl = dynamic_cast<PointLight*>(light);
				Vector3f posViewSpace = (Camera::getMainCamera()->getViewMatrix() * Vector4f(dl->transform->getPositionWorld(), 1)).xyz();
				++i;
				_deferredShader->setUniform("pointLights[" + std::to_string(i) + "].position", posViewSpace);
				_deferredShader->setUniform("pointLights[" + std::to_string(i) + "].color", dl->color);
				_deferredShader->setUniform("pointLights[" + std::to_string(i) + "].intensity", dl->intensity);
				_deferredShader->setUniform("pointLights[" + std::to_string(i) + "].attenuation.constant", dl->constant);
				_deferredShader->setUniform("pointLights[" + std::to_string(i) + "].attenuation.linear", dl->linear);
				_deferredShader->setUniform("pointLights[" + std::to_string(i) + "].attenuation.quadratic", dl->quadratic);
			}
		}
	}

	// Render opaque objects
	_gBuffer->render(_deferredMaterial);


	// Render Skybox
	_skyBox.render();

	// Render transparent objects
	renderTransparents();


	// Directly draw to main buffer if no post processing is active
	if (!PostProcessing::isActive())
	{
		FrameBuffer::resetDefaultBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		_mainFrameBuffer->render("color");
	}
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
		_mainFrameBuffer->enable();
		_mainFrameBuffer->clear();
	}

	_skyBox.render();
	
	if (captureMode)
	{
		for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
		{
			if ((*it)->getEnable() && Camera::getMainCamera()->frustumTest(*it))
			{
				if (!(*it)->getReflected && captureMode) continue;


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
						_skyBox.getLightMap()->irradianceMap->bind(_ambientLightShader->textures["irradianceMap"].unit);
						_skyBox.getLightMap()->specularMap->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);
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
	}
	

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
				_skyBox.getLightMap()->specularMap->bind(_transparentBaseShader->textures["convolutedSpecularMap"].unit);
				_transparentBaseShader->setUniform("useCorrection", false);
			}


			// AMBIENT
			AmbientLight* ambient = Lighting::getLights<AmbientLight>()[0];

			glEnable(GL_BLEND);
			/*
			if (it->second->cutout)
			{
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			else
				*/
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


void Renderer::renderBase()
{
	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
	{
		if ((*it)->getEnable() && Camera::getMainCamera()->frustumTest(*it))
		{
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
				if (lp != nullptr)
				{
					lp->getIrradianceCube()->bind(_ambientLightShader->textures["irradianceMap"].unit);
					lp->getSpecularCube()->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);

					_ambientLightShader->setUniform("minBound", lp->bounds.getMinBoundGlobal());
					_ambientLightShader->setUniform("maxBound", lp->bounds.getMaxBoundGlobal());
					_ambientLightShader->setUniform("boundPos", lp->bounds.getBoundPosition());
					_ambientLightShader->setUniform("useCorrection", true);
				}
				else
				{
					_skyBox.getLightMap()->irradianceMap->bind(_ambientLightShader->textures["irradianceMap"].unit);
					_skyBox.getLightMap()->specularMap->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);
					_ambientLightShader->setUniform("useCorrection", false);
				}

				// AMBIENT
				AmbientLight* ambient = Lighting::getLights<AmbientLight>()[0];
				(*it)->render(_ambientLightShader, ambient);
			}
		}
	}

}

void Renderer::renderTransparents()
{
	updateTransparentQueue();
	for (std::map<float, TransparentMeshRenderer*>::reverse_iterator it = _renderQueueTransparent.rbegin(); it != _renderQueueTransparent.rend(); ++it)
	{
		glEnable(GL_DEPTH_TEST);

		// IBL
		_transparentBaseShader->bind();
		LightProbe* lp = Lighting::findInLightProberVolume(it->second->transform);
		if (lp != nullptr)
		{
			lp->getSpecularCube()->bind(_transparentBaseShader->textures["convolutedSpecularMap"].unit);

			_transparentBaseShader->setUniform("minBound", lp->bounds.getMinBoundGlobal());
			_transparentBaseShader->setUniform("maxBound", lp->bounds.getMaxBoundGlobal());
			_transparentBaseShader->setUniform("boundPos", lp->bounds.getBoundPosition());
			_transparentBaseShader->setUniform("useCorrection", true);
		}
		else
		{
			_skyBox.getLightMap()->specularMap->bind(_transparentBaseShader->textures["convolutedSpecularMap"].unit);
			_transparentBaseShader->setUniform("useCorrection", false);
		}


		// AMBIENT
		AmbientLight* ambient = Lighting::getLights<AmbientLight>()[0];

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);

		
		it->second->render(_transparentBaseShader, ambient);

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


void Renderer::renderAmbient(int pass, LightMap* lightmap, AABB* volume)
{
	_skyBox.render();

	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
	{
		if ((*it)->getEnable() && Camera::getMainCamera()->frustumTest(*it))
		{
			if (!(*it)->entity->getStatic()) continue;


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
		if ((*it)->getEnable() && (!(!(*it)->getReflected && captureMode)))
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
	_gizmoBuffer->enable();


	if (_debugMode >= 1)
	{
		SceneManager::getCurrentScene()->getEntities();
		for (std::vector<Entity*>::iterator it = SceneManager::getCurrentScene()->getEntities().begin(); it != SceneManager::getCurrentScene()->getEntities().end(); ++it)
		{
			if ((*it)->getEnable())
			{
				//(*it)->transform->renderHandels();
			}
		}
	}

	if (_debugMode >= 2)
	{

		if (_quadVAO == -1)
		{
			glGenVertexArrays(1, &_quadVAO);
			glBindVertexArray(_quadVAO);

			glGenBuffers(1, &_quadVBO);
		}

		glBindVertexArray(_quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);

		glBufferData(GL_ARRAY_BUFFER, stuff.size() * sizeof(float), &stuff[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
		glVertexAttribPointer(Vertex::AttibLocation::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
		glVertexAttribPointer(Vertex::AttibLocation::COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));


		CircleShader* shader = CircleShader::getInstance();
		shader->bind();
		shader->enableAttributes();
		shader->update(nullptr);


		//glDisable(GL_DEPTH_TEST);

		glEnable(GL_PROGRAM_POINT_SIZE);
		//glPointSize(1);

		glDrawArrays(GL_POINTS, 0, stuff.size() / 6);

		glDisable(GL_PROGRAM_POINT_SIZE);
		shader->disableAttributes();

		//glEnable(GL_DEPTH_TEST);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
		{
			if ((*it)->getEnable())
			{
				//MeshRenderer* mr = (MeshRenderer*)(*it);
				//draw_bbox(mr->getMesh(), mr->transform->getTransformationMatrix());
			
				/*
				Matrix4f projectionMatrix = Camera::getMainCamera()->getProjectionMatrix();
				//glMatrixMode(GL_PROJECTION);
				//glLoadMatrixf((const GLfloat*)&projectionMatrix(0, 0));
				//glMatrixMode(GL_MODELVIEW);
				Matrix4f MVP = Camera::getMainCamera()->getViewProjection() * mr->transform->getTransformationMatrix();
				//glLoadMatrixf((const GLfloat*)&MV(0, 0));

				Vector3f min = (MVP * Vector4f(mr->getMesh()->min, 1.0f)).xyz();
				Vector3f max = (MVP * Vector4f(mr->getMesh()->max, 1.0f)).xyz();

				Vector3f x = min + Vector3f::UNIT_X;
				Vector3f z = min + Vector3f::UNIT_Z;
				Vector3f y = min + Vector3f::UNIT_Y;

				Vector3f negx = (min + Vector3f::UNIT_X) * -1;
				Vector3f negz = (min + Vector3f::UNIT_Z) * -1;
				Vector3f negy = (min + Vector3f::UNIT_Y) * -1;
				*/
				//Vector2f min = Camera::getMainCamera()->worldToScreen(((*it)->transform->getTransformationMatrix() * Vector4f((*it)->getBoundingSphere().getCenter(), 1)).xyz());
			}
		}
	}


	if (_debugMode >= 3)
	{
		ImGui::Begin("GBuffer");
		ImGui::SliderFloat("Scale", &factor, 0.1f, 1);
		for (size_t i = 0; i < 8; ++i)
		{
			ImGui::Text((buffers[i] + "  id: " + std::to_string(_gBuffer->getAttachment(buffers[i])->id) + "  index: " + std::to_string(_gBuffer->getAttachment(buffers[i])->attachmentIndex)).c_str() );
			bool close = true;
			if (ImGui::ImageButton((void *)_gBuffer->getAttachment(buffers[i])->id, ImVec2(_gBuffer->getWidth() * factor, _gBuffer->getHeight() * factor), ImVec2(0, 1), ImVec2(1, 0)))
			{
				ImGui::OpenPopup(buffers[i].c_str());
			}
			if (ImGui::BeginPopup(buffers[i].c_str()))
			{				
				float bigFactor = 1.2f;
				ImGui::Image((void *)_gBuffer->getAttachment(buffers[i])->id, ImVec2(_gBuffer->getWidth() / bigFactor, _gBuffer->getHeight() / bigFactor), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::EndPopup();
			}
			ImGui::NewLine();
		}
		ImGui::End();
	}
	

	FrameBuffer::resetDefaultBuffer();
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_gizmoBuffer->render("color");

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	

	_gizmoBuffer->enable();
	_gizmoBuffer->clear();
}

void Renderer::updateTransparentQueue()
{
	float bias = 0.0001f;
	for (std::vector<ObjectRenderer*>::iterator it = _renderListTransparent.begin(); it != _renderListTransparent.end(); ++it)
	{
		if ((*it)->getEnable() && (*it)->entity->getStatic() && Camera::getMainCamera()->frustumTest(*it))
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

void Renderer::setDebug(int debugMode)
{
	_debugMode = debugMode;
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
