#include "headers/ModelRenderer.h"
#include "headers/Configuration.h"

/* Public functions */

ModelRenderer::ModelRenderer()
{
	_renderQueue = RenderQueuePtr(new RenderQueue);
}

ModelRenderer::ModelRenderer(ObjectManagerPtr o)
{
	_renderQueue = RenderQueuePtr(new RenderQueue);
	_objectManager = o;
}

ObjectManagerPtr ModelRenderer::getObjectManager()
{
	return _objectManager;
}

void ModelRenderer::setObjectManager(ObjectManagerPtr o)
{
	_objectManager = o;
}

RenderQueuePtr ModelRenderer::getRenderQueue()
{
	return _renderQueue;
}

void ModelRenderer::drawModel(const std::string &modelName, const std::string &cameraName, const vmml::Matrix4f & modelMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling, bool cullIndividualGeometry)
{
	drawModel(_objectManager->getModel(modelName), modelMatrix, _objectManager->getCamera(cameraName)->getViewMatrix(), _objectManager->getCamera(cameraName)->getProjectionMatrix(), lightNames, doFrustumCulling, cullIndividualGeometry);
}

void ModelRenderer::drawModel(ModelPtr model, const vmml::Matrix4f &modelMatrix, const vmml::Matrix4f &viewMatrix, const vmml::Matrix4f &projectionMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling, bool cullIndividualGeometry)
{
	vmml::Matrix4f modelViewMatrix = viewMatrix*modelMatrix;
	vmml::Matrix4f modelViewProjectionMatrix = projectionMatrix*modelViewMatrix;
	vmml::Visibility visibility = vmml::VISIBILITY_FULL;

	GLint compareShader = -1;

	// Frustum culling
	if (doFrustumCulling){
		visibility = viewFrustumCulling(model->getBoundingBoxObjectSpace(), modelViewProjectionMatrix);
		//if (visibility == vmml::VISIBILITY_NONE) 	bRenderer::log(modelName + " was culled");
	}

	// Draw geometry
	if (visibility != vmml::VISIBILITY_NONE){
		Model::GroupMap &groupsModel = model->getGroups();
		for (auto i = groupsModel.begin(); i != groupsModel.end(); ++i)
		{
			GeometryPtr geometry = i->second;

			// Only do frustum culling for the geometry if the model has more than one geometry object, 
			// otherwise the culling result of the whole model is the same as for the geometry
			if (doFrustumCulling && cullIndividualGeometry && groupsModel.size() > 1){
				visibility = viewFrustumCulling(geometry->getBoundingBoxObjectSpace(), modelViewProjectionMatrix);
				//if (visibility == vmml::VISIBILITY_NONE) 	bRenderer::log(modelName + " was culled");
			}

			if (visibility != vmml::VISIBILITY_NONE){
				ShaderPtr shader = geometry->getMaterial()->getShader();

				// Only pass Uniforms if the shader of the current geometry differs from the one of the last
				if (shader && compareShader != shader->getProgramID())
				{
					compareShader = shader->getProgramID();

					shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_PROJECTION_MATRIX(), projectionMatrix);
					shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX(), modelViewMatrix);

					// Lighting
					if (shader->supportsDiffuseLighting() || shader->supportsSpecularLighting()){
						GLfloat numLights = static_cast<GLfloat>(lightNames.size());
						bool variableNumberOfLights = shader->supportsVariableNumberOfLights();
						GLuint maxLights = shader->getMaxLights();
						if (numLights > maxLights)
							numLights = static_cast<GLfloat>(maxLights);

						if (variableNumberOfLights && numLights > 0)
							shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_NUMBER_OF_LIGHTS(), numLights);
						for (int i = 0; i < numLights; i++){
							std::string pos = std::to_string(i);
							LightPtr l = _objectManager->getLight(lightNames[i]);
							shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE() + pos, (viewMatrix*l->getPosition()));
							if (shader->supportsDiffuseLighting())
								shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_DIFFUSE_LIGHT_COLOR() + pos, l->getDiffuseColor());
							if (shader->supportsSpecularLighting())
								shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_SPECULAR_LIGHT_COLOR() + pos, l->getSpecularColor());
							shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_LIGHT_INTENSITY() + pos, l->getIntensity());
							shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_LIGHT_ATTENUATION() + pos, l->getAttenuation());
							shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_LIGHT_RADIUS() + pos, l->getRadius());
						}
					}
					// Ambient
					if (shader->supportsAmbientLighting())
						shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR(), _objectManager->getAmbientColor());
				}

				geometry->draw();
			}
		}
	}
}

void ModelRenderer::queueModelInstance(const std::string &modelName, const std::string &instanceName, const std::string &cameraName, const vmml::Matrix4f & modelMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling, bool cullIndividualGeometry, bool isTransparent, GLenum blendSfactor, GLenum blendDfactor, GLfloat customDistance)
{
	queueModelInstance(_objectManager->getModel(modelName), instanceName, modelMatrix, _objectManager->getCamera(cameraName)->getViewMatrix(), _objectManager->getCamera(cameraName)->getProjectionMatrix(), lightNames, doFrustumCulling, cullIndividualGeometry, isTransparent, blendSfactor, blendDfactor, customDistance);
}

void ModelRenderer::queueModelInstance(ModelPtr model, const std::string &instanceName, const vmml::Matrix4f &modelMatrix, const vmml::Matrix4f &viewMatrix, const vmml::Matrix4f &projectionMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling, bool cullIndividualGeometry, bool isTransparent, GLenum blendSfactor, GLenum blendDfactor, GLfloat customDistance)
{
	vmml::Matrix4f modelViewMatrix = viewMatrix*modelMatrix;
	vmml::Matrix4f modelViewProjectionMatrix = projectionMatrix*modelViewMatrix;
	vmml::Visibility visibility = vmml::VISIBILITY_FULL;

	// Frustum culling
	if (doFrustumCulling){
		visibility = viewFrustumCulling(model->getBoundingBoxObjectSpace(), modelViewProjectionMatrix);
		//if (visibility == vmml::VISIBILITY_NONE) 	bRenderer::log(modelName + " was culled");
	}

	// Queue model if visible
	if (visibility != vmml::VISIBILITY_NONE){

		// Update instance properties
		Model::InstanceMapPtr instanceMap = model->addInstance(instanceName);
		for (auto i = instanceMap->begin(); i != instanceMap->end(); ++i)
		{
			ShaderPtr shader = i->first;
			PropertiesPtr properties = i->second;

			properties->setMatrix(bRenderer::DEFAULT_SHADER_UNIFORM_PROJECTION_MATRIX(), projectionMatrix);
			properties->setMatrix(bRenderer::DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX(), modelViewMatrix);

			// Lighting
			if (shader->supportsDiffuseLighting() || shader->supportsSpecularLighting()){
				GLfloat numLights = static_cast<GLfloat>(lightNames.size());
				bool variableNumberOfLights = shader->supportsVariableNumberOfLights();
				GLuint maxLights = shader->getMaxLights();
				if (numLights > maxLights)
					numLights = static_cast<GLfloat>(maxLights);

				if (variableNumberOfLights && numLights > 0)
					properties->setScalar(bRenderer::DEFAULT_SHADER_UNIFORM_NUMBER_OF_LIGHTS(), numLights);
				for (int i = 0; i < numLights; i++){
					std::string pos = std::to_string(i);
					LightPtr l = _objectManager->getLight(lightNames[i]);
					properties->setVector(bRenderer::DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE() + pos, (viewMatrix*l->getPosition()));
					if (shader->supportsDiffuseLighting())
						properties->setVector(bRenderer::DEFAULT_SHADER_UNIFORM_DIFFUSE_LIGHT_COLOR() + pos, l->getDiffuseColor());
					if (shader->supportsSpecularLighting())
						properties->setVector(bRenderer::DEFAULT_SHADER_UNIFORM_SPECULAR_LIGHT_COLOR() + pos, l->getSpecularColor());
					properties->setScalar(bRenderer::DEFAULT_SHADER_UNIFORM_LIGHT_INTENSITY() + pos, l->getIntensity());
					properties->setScalar(bRenderer::DEFAULT_SHADER_UNIFORM_LIGHT_ATTENUATION() + pos, l->getAttenuation());
					properties->setScalar(bRenderer::DEFAULT_SHADER_UNIFORM_LIGHT_RADIUS() + pos, l->getRadius());
				}
			}
			// Ambient
			if (shader->supportsAmbientLighting())
				properties->setVector(bRenderer::DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR(), _objectManager->getAmbientColor());
		}

		// Queue geometry
		Model::GroupMap &groupsModel = model->getGroups();
		for (auto i = groupsModel.begin(); i != groupsModel.end(); ++i)
		{
			std::string geometryName = i->first;
			GeometryPtr geometry = i->second;

			// Only do frustum culling for the geometry if the model has more than one geometry object, 
			// otherwise the culling result of the whole model is the same as for the geometry
			if (doFrustumCulling && cullIndividualGeometry && groupsModel.size() > 1){
				visibility = viewFrustumCulling(geometry->getBoundingBoxObjectSpace(), modelViewProjectionMatrix);
				//if (visibility == vmml::VISIBILITY_NONE) 	bRenderer::log(modelName + " was culled");
			}

			if (visibility != vmml::VISIBILITY_NONE){
				GLuint programID = geometry->getMaterial()->getShader()->getProgramID();

				if (isTransparent){
					// Find out distance
					GLfloat distance = customDistance;
					if (distance > 9999.0f){
						vmml::Vector3f centerViewSpace = (modelViewProjectionMatrix * geometry->getBoundingBoxObjectSpace().getCenter());
						distance = centerViewSpace.z();
					}
					_renderQueue->submitToRenderQueue(programID, geometry->getMaterial()->getName(), geometryName, instanceName, geometry, distance, isTransparent, blendSfactor, blendDfactor);
				}
				else
					_renderQueue->submitToRenderQueue(programID, geometry->getMaterial()->getName(), geometryName, instanceName, geometry);
			}
		}
	}
}

void ModelRenderer::drawText(const std::string &textSpriteName, const std::string &cameraName, const vmml::Matrix4f &modelMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling)
{
	drawModel(_objectManager->getTextSprite(textSpriteName), modelMatrix, _objectManager->getCamera(cameraName)->getViewMatrix(), _objectManager->getCamera(cameraName)->getProjectionMatrix(), lightNames, doFrustumCulling, false);
}

void ModelRenderer::queueTextInstance(const std::string &textSpriteName, const std::string &instanceName, const std::string &cameraName, const vmml::Matrix4f &modelMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling, GLenum blendSfactor, GLenum blendDfactor, GLfloat customDistance)
{
	queueModelInstance(_objectManager->getTextSprite(textSpriteName), instanceName, modelMatrix, _objectManager->getCamera(cameraName)->getViewMatrix(), _objectManager->getCamera(cameraName)->getProjectionMatrix(), lightNames, doFrustumCulling, false, true, blendSfactor, blendDfactor, customDistance);
}

void ModelRenderer::drawQueue(GLenum mode)
{
	_renderQueue->draw(mode);
}

void ModelRenderer::clearQueue()
{
	_renderQueue->clear();
}

vmml::Visibility ModelRenderer::viewFrustumCulling(const vmml::AABBf &aabbObjectSpace, const vmml::Matrix4f &modelViewProjectionMatrix)
{
	vmml::FrustumCullerf culler;
	culler.setup(modelViewProjectionMatrix);
	return culler.test_aabb(vmml::Vector2f(aabbObjectSpace.getMin().x(), aabbObjectSpace.getMax().x()), vmml::Vector2f(aabbObjectSpace.getMin().y(), aabbObjectSpace.getMax().y()), vmml::Vector2f(aabbObjectSpace.getMin().z(), aabbObjectSpace.getMax().z()));
}
