#include "MeshRenderer.h"
#include "../Core/Display.h"
#include "Transform.h"
#include "Camera.h"
#include "../Core/Systems/Renderer.h"
#include "../Core/Systems/Lighting.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "../Core/GameObject.h"

void MeshRenderer::init()
{
	GLenum drawMode = 0;
	if (gameObject->getStatic())
		drawMode = GL_STATIC_DRAW;
	else
		drawMode = GL_DYNAMIC_DRAW;

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, _mesh->vertices.size() * sizeof(Vertex), &(_mesh->vertices[0]), drawMode);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh->indices.size() * sizeof(unsigned int), &(_mesh->indices[0]), drawMode);

	glVertexAttribPointer(Vertex::AttibLocation::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(Vertex::AttibLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(Vertex::AttibLocation::COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(Vertex::AttibLocation::TEXTURECOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glVertexAttribPointer(Vertex::AttibLocation::TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);


	default_normal = new Texture("./res/default_normal.png", false);
	default_specular = new Texture("./res/default_specular.png", false);
}

void MeshRenderer::render()
{
	if (Renderer::renderMode == Renderer::RenderMode::CUSTOM) {
		_material->shader->bind();
		if (_material->albedoMap != nullptr)
			_material->albedoMap->bind();
		else
			glBindTexture(GL_TEXTURE_2D, 0);

		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		_material->shader->update(mvp);

		glBindVertexArray(_vao);

		_material->shader->enableAttributes();
		glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
		_material->shader->enableAttributes();

		glBindVertexArray(0);
	}else if (Renderer::renderMode == Renderer::RenderMode::FORWARD) {
	
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		Matrix4f modelMatrix = transform->getTransformationMatrix();
		Vector4f eyePos = Vector4f(Camera::getMainCamera()->transform->getPositionWorld(), 1);
		
		glBindVertexArray(_vao);

		if (_material->albedoMap != nullptr)
			_material->albedoMap->bind();
		else
			glBindTexture(GL_TEXTURE_2D, 0);
	
		
		
		// AMBIENT
		AmbientLight* ambient = Lighting::getLights<AmbientLight>()[0];
		_material->forwardShader->bindAmbientShader();
		_material->forwardShader->ambientLightShader->update(mvp, *ambient);
		_material->forwardShader->ambientLightShader->enableAttributes();
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
			glUniform1i(_material->forwardShader->directionalLightShader->uniforms["normalMap"], GL_TEXTURE0);
			glUniform1i(_material->forwardShader->directionalLightShader->uniforms["specularMap"], GL_TEXTURE1);
			_material->forwardShader->directionalLightShader->update(mvp, modelMatrix, *directionalLights[i], eyePos);
			_material->forwardShader->directionalLightShader->enableAttributes();

			if (_material->normalMap != nullptr) _material->normalMap->bind(GL_TEXTURE0);
			else default_normal->bind(GL_TEXTURE0);

			if (_material->specularMap != nullptr) _material->specularMap->bind(GL_TEXTURE1);
			else default_specular->bind(GL_TEXTURE1);

			glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
			_material->forwardShader->directionalLightShader->disableAttributes();
		}
		


		

		// POINT LIGHTS
		std::vector<PointLight*> pointLights = Lighting::getLights<PointLight>();
		for(size_t i = 0; i < pointLights.size(); ++i){
			if (!pointLights[i]->getEnable()) continue;
			_material->forwardShader->bindPointLightShader();
			glUniform1i(_material->forwardShader->pointLightShader->uniforms["normalMap"], 0);
			glUniform1i(_material->forwardShader->pointLightShader->uniforms["specularMap"], 1);
			_material->forwardShader->pointLightShader->update(mvp, modelMatrix, *pointLights[i], eyePos);
			_material->forwardShader->pointLightShader->enableAttributes();
			
			if (_material->normalMap != nullptr) _material->normalMap->bind(GL_TEXTURE0);
			else default_normal->bind(GL_TEXTURE0);

			if (_material->specularMap != nullptr) _material->specularMap->bind(GL_TEXTURE1);
			else default_specular->bind(GL_TEXTURE1);

			glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
			_material->forwardShader->pointLightShader->enableAttributes();
		}

		// SPOT LIGHTS

		
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
		

		glBindVertexArray(0);
	}
}

void MeshRenderer::onDestroy()
{
	glDeleteVertexArrays(1, &_vao);
}