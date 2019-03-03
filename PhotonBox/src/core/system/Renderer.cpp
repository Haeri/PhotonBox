#include "PhotonBox/core/system/Renderer.h"

#include <algorithm>
#include <string>
#include <iostream>

#include "PhotonBox/core/Display.h"
#include "PhotonBox/core/system/Lighting.h"
#include "PhotonBox/core/system/SceneManager.h"
#include "PhotonBox/core/system/PostProcessing.h"
#include "PhotonBox/core/system/DebugGUI.h"
#include "PhotonBox/core/Entity.h"
#include "PhotonBox/component/AmbientLight.h"
#include "PhotonBox/component/Camera.h"
#include "PhotonBox/component/DirectionalLight.h"
#include "PhotonBox/component/ObjectRenderer.h"
#include "PhotonBox/component/MeshRenderer.h"
#include "PhotonBox/component/PointLight.h"
#include "PhotonBox/component/SpotLight.h"
#include "PhotonBox/component/Transform.h"
#include "PhotonBox/component/LightProbe.h"
#include "PhotonBox/resource/FrameBuffer.h"
#include "PhotonBox/resource/LightMap.h"
#include "PhotonBox/resource/PostProcessor.h"
#include "PhotonBox/resource/Texture.h"
#include "PhotonBox/resource/Scene.h"
#include "PhotonBox/resource/Material.h"
#include "PhotonBox/resource/shader/LitShader.h"
#include "PhotonBox/resource/shader/VolumetricFogShader.h"
#include "PhotonBox/resource/shader/DirectionalShadowShader.h"
#include "PhotonBox/resource/shader/GShader.h"
#include "PhotonBox/resource/shader/TransparentShader.h"
#include "PhotonBox/resource/shader/DeferredShader.h"
#include "PhotonBox/resource/shader/DepthShader.h"
#include "PhotonBox/resource/shader/CircleShader.h"
#include "PhotonBox/resource/shader/ForwardDirectionalLightShader.h"
#include "PhotonBox/util/GLError.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

int Renderer::_debugMode;
bool Renderer::_shadowsAreDirty = true;
SkyBox Renderer::_skyBox;
std::vector<ObjectRenderer*> Renderer::_renderListOpaque;
std::vector<ObjectRenderer*> Renderer::_renderListTransparent;
std::vector<ObjectRenderer*> Renderer::_renderListCustom;
std::map<float, MeshRenderer*> Renderer::_renderQueueTransparent;
int Renderer::_drawCalls;
Texture* Renderer::_noise;

ForwardAmbientLightShader* Renderer::_ambientLightShader;
ForwardDirectionalLightShader* Renderer::_directionalLightShader;
ForwardPointLightShader* Renderer::_pointLightShader;
ForwardSpotLightShader* Renderer::_spotLightShader;
TransparentShader* Renderer::_transparentBaseShader;
DeferredShader* Renderer::_deferredShader;
DirectionalShadowShader* Renderer::_directionalShadowShader;
VolumetricFogShader* Renderer::_volumetricFogShader;

Material* Renderer::_deferredMaterial;
Material* Renderer::_volumetricFogMaterial;

FrameBuffer* Renderer::_mainFrameBuffer;
FrameBuffer* Renderer::_gBuffer;
FrameBuffer* Renderer::_gizmoBuffer;
FrameBuffer* Renderer::_shadowBuffer;
Vector3f Renderer::_clearColor = Vector3f(0.3f, 0.3f, 0.3f);

// Temp stuff
GLuint _quadVAO = -1;
GLuint _quadVBO;
std::vector<float> stuff;


float factor = 0.1f;
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
	if (type == RenderType::RT_TRANSPARENT)
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
	if (renderer->getType() == RenderType::RT_TRANSPARENT)
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
	_skyBox.setCubeMap(cubeMap);
	_skyBox.setDrawMode(SkyBox::DRAW_CUBE_MAP);
}

void Renderer::init(float superSampling)
{
	std::cout << "Initializing Renderer";
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
	_directionalShadowShader = DirectionalShadowShader::getInstance();
	_volumetricFogShader = VolumetricFogShader::getInstance();
	
	_noise = new Texture(Resources::ENGINE_RESOURCES + "/noise/blue_noise.png", false);

	_mainFrameBuffer = new FrameBuffer(superSampling);
	_mainFrameBuffer->addTextureAttachment("color", true, false);
	_mainFrameBuffer->addDepthBufferAttachment();
	_mainFrameBuffer->ready();

	_gBuffer = new FrameBuffer(superSampling);
	_gBuffer->addTextureAttachment("gPosition", true, true);
	_gBuffer->addTextureAttachment("gNormal", true, true);
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

	_shadowBuffer = new FrameBuffer(SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION);
	_shadowBuffer->addDepthTextureAttachment("depth");
	_shadowBuffer->ready();
	
	_deferredMaterial = new Material(_deferredShader);
	_deferredMaterial->setTexture("gPosition", _gBuffer, "gPosition");
	_deferredMaterial->setTexture("gNormal", _gBuffer, "gNormal");
	_deferredMaterial->setTexture("gRoughness", _gBuffer, "gRoughness");
	_deferredMaterial->setTexture("gMetallic", _gBuffer, "gMetallic");
	_deferredMaterial->setTexture("gAlbedo", _gBuffer, "gAlbedo");
	_deferredMaterial->setTexture("gIrradiance", _gBuffer, "gIrradiance");
	_deferredMaterial->setTexture("gRadiance", _gBuffer, "gRadiance");
	_deferredMaterial->setTexture("gEmission", _gBuffer, "gEmission");
	_deferredMaterial->setTexture("noise", _noise);

	_volumetricFogMaterial = new Material(_volumetricFogShader);
	_volumetricFogMaterial->setTexture("gPosition", _gBuffer, "gPosition");
	_volumetricFogMaterial->setTexture("noise", _noise);


	_debugMode = 0;
	
	float max = 100;
	int half = (int)(max / 2.0f);
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
	std::cout << " - Done\n";
}

void Renderer::reset()
{
	_skyBox.reset();
	_shadowsAreDirty = true;
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

	glEnable(GL_DEPTH_TEST);

	if (depthPrePass)
	{
		glDepthMask(GL_TRUE);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthFunc(GL_LESS);

		for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
		{
			if ((*it)->getEnable() && Camera::getMainCamera()->frustumTest(*it))
			{
				(*it)->render(DepthShader::getInstance());	
			}
		}

		glDepthMask(GL_FALSE);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glDepthFunc(GL_EQUAL);
	}


	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
	{
		if ((*it)->getEnable() && Camera::getMainCamera()->frustumTest(*it))
		{
			if ((*it)->getType() == RenderType::RT_CUTOUT)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			Shader* shader = (*it)->getMaterial()->shader;

			//if(typeid(*shader) == typeid(GShader))
			if (shader->getType() == Shader::Type::SURFACE_SHADER)
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
					_skyBox.getLightMap()->getIrradianceMap()->bind(shader->textures["irradianceMap"].unit);
					_skyBox.getLightMap()->getSpecularConvolutionMap()->bind(shader->textures["convolutedSpecularMap"].unit);
					shader->setUniform("useCorrection", false);
				}
			
				(*it)->render();
			}
			else
			{
				_renderListCustom.push_back((*it));
			}

			if ((*it)->getType() == RenderType::RT_CUTOUT)
			{
				glDisable(GL_BLEND);
			}
		}
	}
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::clearDrawCalls()
{
	_drawCalls = 0;
}

void Renderer::renderShadows()
{
	_shadowBuffer->enable();
	_shadowBuffer->clear();
	
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);

	std::vector<DirectionalLight*>* directionalLights = Lighting::getLights<DirectionalLight>();
	if (directionalLights != nullptr) {
		for (size_t i = 0; i < directionalLights->size(); ++i)
		{
			if (!directionalLights->at(i)->getEnable()) continue;

			for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
			{
				if ((*it)->getEnable() && (*it)->castShadows && Camera::getMainCamera()->frustumTest(*it))
				{
					(*it)->render(_directionalShadowShader, directionalLights->at(i));
				}
			}


			/*
			if(!captureMode){
			ImGui::Begin("Depth");
			ImGui::Image((ImTextureID)shadowBuffer->getAttachment("depth")->id, ImVec2(_shadowMapResolution/15.0f, _shadowMapResolution/15.0f), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
			}
			*/
		}
	}
	glCullFace(GL_BACK);
}

void Renderer::renderDeferred()
{	
	// TODO: Replace with a system that detects change in scene
	if(_shadowsAreDirty) {
		// Render Shadows
		renderShadows();
		_shadowsAreDirty = false;
	}

	// Clear Buffer
	_mainFrameBuffer->enable();
	_mainFrameBuffer->clear();

	// Send light data to shader
	std::unordered_map<std::type_index, std::vector<LightEmitter*>> lights = Lighting::getAllLights();
	

	_deferredShader->bind();
	_deferredShader->setUniform<int>("numDirectionalLights", (int)(lights[typeid(DirectionalLight)].size()));
	_deferredShader->setUniform<int>("numPointLights", lights[typeid(PointLight)].size());
	_deferredShader->setUniform<int>("numSpotLights", lights[typeid(SpotLight)].size());

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

				_shadowBuffer->bind(_deferredShader->textures["shadowMap"].unit, "depth");
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
			else if (typeid(*light) == typeid(SpotLight))
			{
				SpotLight* dl = dynamic_cast<SpotLight*>(light);
				Vector3f posViewSpace = (Camera::getMainCamera()->getViewMatrix() * Vector4f(dl->transform->getPositionWorld(), 1)).xyz();
				Vector3f directionView = (Camera::getMainCamera()->getViewMatrix() * Vector4f(dl->transform->forwardWorld(), 0.0f)).xyz();

				++i;
				_deferredShader->setUniform("spotLights[" + std::to_string(i) + "].position", posViewSpace);
				_deferredShader->setUniform("spotLights[" + std::to_string(i) + "].direction", directionView);
				_deferredShader->setUniform("spotLights[" + std::to_string(i) + "].coneAngle", dl->coneAngle);
				_deferredShader->setUniform("spotLights[" + std::to_string(i) + "].coneFallOff", dl->coneAttenuation);
				_deferredShader->setUniform("spotLights[" + std::to_string(i) + "].color", dl->color);
				_deferredShader->setUniform("spotLights[" + std::to_string(i) + "].intensity", dl->intensity);
				_deferredShader->setUniform("spotLights[" + std::to_string(i) + "].attenuation.constant", dl->constant);
				_deferredShader->setUniform("spotLights[" + std::to_string(i) + "].attenuation.linear", dl->linear);
				_deferredShader->setUniform("spotLights[" + std::to_string(i) + "].attenuation.quadratic", dl->quadratic);
			}
		}
	}

	// Render opaque objects
	_gBuffer->render(_deferredMaterial);

	// Blit Depth
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _gBuffer->getFBO());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _mainFrameBuffer->getFBO());

	glBlitFramebuffer(0, 0, _gBuffer->getWidth(), _gBuffer->getHeight(), 0, 0, _mainFrameBuffer->getWidth(), _mainFrameBuffer->getHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);


	// Render Skybox
	_skyBox.render();

	// Render miscelenious
	renderCustoms();

	// Render transparent objects
	renderTransparents();

	// Render volumetric fog
	renderFog();
	

	// Directly draw to main buffer if no post processing is active
	if (!PostProcessing::isActive())
	{
		FrameBuffer::resetDefaultBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		_mainFrameBuffer->render("color");
	}
}

void Renderer::renderForward()
{
	// Bind & clear Shadow FBO
	renderShadows();

	// Bind & clear Main FBO
	_mainFrameBuffer->enable();
	_mainFrameBuffer->clear();

	_skyBox.render();
	
	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
	{
		if ((*it)->getEnable() && Camera::getMainCamera()->frustumTest(*it))
		{
			if ((*it)->getMaterial() != nullptr && (*it)->getMaterial()->shader == GShader::getInstance())
//			if (typeid((**it)) != typeid(MeshRenderer) || ((*it)->getMaterial() != nullptr && (*it)->getMaterial()->shader != nullptr))
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
					_skyBox.getLightMap()->getIrradianceMap()->bind(_ambientLightShader->textures["irradianceMap"].unit);
					_skyBox.getLightMap()->getSpecularConvolutionMap()->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);
					_ambientLightShader->setUniform("useCorrection", false);
				}

				// AMBIENT
				AmbientLight* ambient = Lighting::getLights<AmbientLight>()->at(0);
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
						
						if (typeid(*light) == typeid(DirectionalLight))
						{
							ForwardDirectionalLightShader::getInstance()->bind();
							Renderer::getShadowBuffer()->bind(ForwardDirectionalLightShader::getInstance()->textures["shadowMap"].unit, "depth");
						}
						(*it)->render(light->getLightShader(), light);
					}
				}

				glDepthMask(GL_TRUE);
				glDepthFunc(GL_LESS);
				glDisable(GL_BLEND);

			}
			else
			{
				glEnable(GL_DEPTH_TEST);
				(*it)->render();
			}
		}
		
	}
	
	// Render transparents
	renderTransparents();

	if (!PostProcessing::isActive())
	{
		FrameBuffer::resetDefaultBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		_mainFrameBuffer->render("color");
	}
}

void Renderer::renderTransparents()
{
	updateTransparentQueue();
	for (std::map<float, MeshRenderer*>::reverse_iterator it = _renderQueueTransparent.rbegin(); it != _renderQueueTransparent.rend(); ++it)
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
			_skyBox.getLightMap()->getSpecularConvolutionMap()->bind(_transparentBaseShader->textures["convolutedSpecularMap"].unit);
			_transparentBaseShader->setUniform("useCorrection", false);
		}


		// AMBIENT
		AmbientLight* ambient = Lighting::getLights<AmbientLight>()->at(0);
		std::unordered_map<std::type_index, std::vector<LightEmitter*>> lights = Lighting::getAllLights();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		it->second->render(_transparentBaseShader, ambient);


		glDepthFunc(GL_EQUAL);
		glBlendFunc(GL_ONE, GL_ONE);

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
		glEnable(GL_CULL_FACE);

	}
	clearTransparentQueue();
}

void Renderer::renderFog() 
{
	// Send light data to shader
	std::unordered_map<std::type_index, std::vector<LightEmitter*>> lights = Lighting::getAllLights();


	_volumetricFogShader->bind();
	_volumetricFogShader->setUniform<int>("numDirectionalLights", lights[typeid(DirectionalLight)].size());
	_volumetricFogShader->setUniform<int>("numPointLights", lights[typeid(PointLight)].size());
	_volumetricFogShader->setUniform<int>("numSpotLights", lights[typeid(SpotLight)].size());

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
				_volumetricFogShader->setUniform("directionalLights[" + std::to_string(i) + "].lightSpaceMatrix", lightSpaceMatrix);
				_volumetricFogShader->setUniform("directionalLights[" + std::to_string(i) + "].direction", lightViewDirection);
				_volumetricFogShader->setUniform("directionalLights[" + std::to_string(i) + "].color", dl->color);
				_volumetricFogShader->setUniform("directionalLights[" + std::to_string(i) + "].intensity", dl->intensity);

				_shadowBuffer->bind(_volumetricFogShader->textures["shadowMap"].unit, "depth");
			}
			else if (typeid(*light) == typeid(PointLight))
			{
				PointLight* dl = dynamic_cast<PointLight*>(light);
				Vector3f posViewSpace = (Camera::getMainCamera()->getViewMatrix() * Vector4f(dl->transform->getPositionWorld(), 1)).xyz();
				++i;
				_volumetricFogShader->setUniform("pointLights[" + std::to_string(i) + "].position", posViewSpace);
				_volumetricFogShader->setUniform("pointLights[" + std::to_string(i) + "].color", dl->color);
				_volumetricFogShader->setUniform("pointLights[" + std::to_string(i) + "].intensity", dl->intensity);
				_volumetricFogShader->setUniform("pointLights[" + std::to_string(i) + "].attenuation.constant", dl->constant);
				_volumetricFogShader->setUniform("pointLights[" + std::to_string(i) + "].attenuation.linear", dl->linear);
				_volumetricFogShader->setUniform("pointLights[" + std::to_string(i) + "].attenuation.quadratic", dl->quadratic);
			}
			else if (typeid(*light) == typeid(SpotLight))
			{
				SpotLight* dl = dynamic_cast<SpotLight*>(light);
				Vector3f posViewSpace = (Camera::getMainCamera()->getViewMatrix() * Vector4f(dl->transform->getPositionWorld(), 1)).xyz();
				Vector3f directionView = (Camera::getMainCamera()->getViewMatrix() * Vector4f(dl->transform->forwardWorld(), 0.0f)).xyz();

				++i;
				_volumetricFogShader->setUniform("spotLights[" + std::to_string(i) + "].position", posViewSpace);
				_volumetricFogShader->setUniform("spotLights[" + std::to_string(i) + "].direction", directionView);
				_volumetricFogShader->setUniform("spotLights[" + std::to_string(i) + "].coneAngle", dl->coneAngle);
				_volumetricFogShader->setUniform("spotLights[" + std::to_string(i) + "].coneFallOff", dl->coneAttenuation);
				_volumetricFogShader->setUniform("spotLights[" + std::to_string(i) + "].color", dl->color);
				_volumetricFogShader->setUniform("spotLights[" + std::to_string(i) + "].intensity", dl->intensity);
				_volumetricFogShader->setUniform("spotLights[" + std::to_string(i) + "].attenuation.constant", dl->constant);
				_volumetricFogShader->setUniform("spotLights[" + std::to_string(i) + "].attenuation.linear", dl->linear);
				_volumetricFogShader->setUniform("spotLights[" + std::to_string(i) + "].attenuation.quadratic", dl->quadratic);
			}
		}
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	_gBuffer->render(_volumetricFogMaterial);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void Renderer::renderCustoms()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	for (std::vector<ObjectRenderer*>::iterator it = _renderListCustom.begin(); it != _renderListCustom.end(); ++it)
	{
		if ((*it)->getEnable() && Camera::getMainCamera()->frustumTest(*it))
		{
			(*it)->render();
		}
	}

	_renderListCustom.clear();
}

void Renderer::captureScene(LightMap* lightmap)
{
	_skyBox.render();

	for (std::vector<ObjectRenderer*>::iterator it = _renderListOpaque.begin(); it != _renderListOpaque.end(); ++it)
	{
		if ((*it)->getEnable() && Camera::getMainCamera()->frustumTest(*it))
		{
			if (!(*it)->getReflected) continue;


			if ((*it)->getMaterial() != nullptr && (*it)->getMaterial()->shader == GShader::getInstance())
				//			if (typeid((**it)) != typeid(MeshRenderer) || ((*it)->getMaterial() != nullptr && (*it)->getMaterial()->shader != nullptr))
			{
				glEnable(GL_DEPTH_TEST);


				// IBL
				_ambientLightShader->bind();
				LightProbe* lp = Lighting::findInLightProberVolume((*it)->transform);
				if (lightmap != nullptr)
				{
					lightmap->getIrradianceMap()->bind(_ambientLightShader->textures["irradianceMap"].unit);
					lightmap->getSpecularConvolutionMap()->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);
					_ambientLightShader->setUniform("useCorrection", false);
				}
				else
				{
					_skyBox.getLightMap()->getIrradianceMap()->bind(_ambientLightShader->textures["irradianceMap"].unit);
					_skyBox.getLightMap()->getSpecularConvolutionMap()->bind(_ambientLightShader->textures["convolutedSpecularMap"].unit);
					_ambientLightShader->setUniform("useCorrection", false);
				}

				// AMBIENT
				AmbientLight* ambient = Lighting::getLights<AmbientLight>()->at(0);
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
						
						if (typeid(*light) == typeid(DirectionalLight))
						{
							ForwardDirectionalLightShader::getInstance()->bind();
							Renderer::getShadowBuffer()->bind(ForwardDirectionalLightShader::getInstance()->textures["shadowMap"].unit, "depth");
						}

						(*it)->render(light->getLightShader(), light);
					}
				}

				glDepthMask(GL_TRUE);
				glDepthFunc(GL_LESS);
				glDisable(GL_BLEND);

			}
			else
			{
				glEnable(GL_DEPTH_TEST);
				(*it)->render();
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
				(*it)->transform->renderHandels();
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
				float bigFactor = 0.4f;
				ImGui::Image((void *)_gBuffer->getAttachment(buffers[i])->id, ImVec2(_gBuffer->getWidth() * bigFactor, _gBuffer->getHeight() * bigFactor), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::EndPopup();
			}
			ImGui::NewLine();
		}
		ImGui::End();



		ImGui::Begin("Settings");
		if (ImGui::Button("Capture Enviroment"))
		{
			Lighting::start();
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

			while (true)
			{
				if (_renderQueueTransparent.find(dist) == _renderQueueTransparent.end())
					break;
				else
					dist += bias;
			}

			_renderQueueTransparent[dist] = dynamic_cast<MeshRenderer*> (*it);
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
	delete _mainFrameBuffer;
	delete _gBuffer;
	delete	_gizmoBuffer;
	delete _shadowBuffer;
	delete _deferredMaterial;
	delete _volumetricFogMaterial;
	delete _noise;

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
