#include "MeshRenderer.h"
#include "../Core/Display.h"
#include "Transform.h"
#include "Camera.h"
#include "../Core/Systems/Renderer.h"
#include "../Core/Systems/Lighting.h"
#include "PointLight.h"

void MeshRenderer::init()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, _mesh->vertices.size() * sizeof(Vertex), &(_mesh->vertices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh->indices.size() * sizeof(unsigned int),
		&(_mesh->indices[0]), GL_STATIC_DRAW);

	if (Renderer::renderMode == Renderer::RenderMode::CUSTOM) {
		// vertex positions
		if (_material->shader->positionAttrib != -1) {
			glEnableVertexAttribArray(_material->shader->positionAttrib);
			glVertexAttribPointer(_material->shader->positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		}

		// vertex normals
		if (_material->shader->normalAttrib != -1) {
			glEnableVertexAttribArray(_material->shader->normalAttrib);
			glVertexAttribPointer(_material->shader->normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		}

		// vertex colors
		if (_material->shader->colorAttrib != -1) {
			glEnableVertexAttribArray(_material->shader->colorAttrib);
			glVertexAttribPointer(_material->shader->colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		}

		// vertex texture coords
		if (_material->shader->uvAttrib != -1) {
			glEnableVertexAttribArray(_material->shader->uvAttrib);
			glVertexAttribPointer(_material->shader->uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		}
	}else if (Renderer::renderMode == Renderer::RenderMode::FORWARD) {
		// vertex positions
		if (_material->forwardShader->positionAttrib != -1) {
			glEnableVertexAttribArray(_material->forwardShader->positionAttrib);
			glVertexAttribPointer(_material->forwardShader->positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		}

		// vertex normals
		if (_material->forwardShader->normalAttrib != -1) {
			glEnableVertexAttribArray(_material->forwardShader->normalAttrib);
			glVertexAttribPointer(_material->forwardShader->normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		}

		// vertex colors
		if (_material->forwardShader->colorAttrib != -1) {
			glEnableVertexAttribArray(_material->forwardShader->colorAttrib);
			glVertexAttribPointer(_material->forwardShader->colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		}

		// vertex texture coords
		if (_material->forwardShader->uvAttrib != -1) {
			glEnableVertexAttribArray(_material->forwardShader->uvAttrib);
			glVertexAttribPointer(_material->forwardShader->uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		}
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

void MeshRenderer::render()
{
	if (Renderer::renderMode == Renderer::RenderMode::CUSTOM) {
		_material->shader->bind();
		if (_material->texture != nullptr)
			_material->texture->bind();

		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		_material->shader->update(mvp);

		glBindVertexArray(_vao);

		//glDrawArrays(GL_TRIANGLES, 0, _mesh->indices.size());
		glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}
	else if (Renderer::renderMode == Renderer::RenderMode::FORWARD) {
		//glEnable(GL_BLEND); 
		//glBlendFunc(GL_ONE, GL_ONE);
		glBindVertexArray(_vao);


		if (_material->texture != nullptr)
			_material->texture->bind();


		// AMBIENT
		//LightEmitter* ambient = Lighting::getLights<LightEmitter>()[0];
		_material->forwardShader->bindAmbientShader();

		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		_material->forwardShader->ambientShader->update(mvp);

		glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);


		/*
		// POINTLIGHTS
		std::vector<PointLight*> pointLights = Lighting::getLights<PointLight>();
		for(size_t i = 0; i < pointLights.size(); ++i){
			_material->forwardShader->bindPointLightShader();
			Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
			_material->forwardShader->pointLightShader->update(mvp, *pointLights[i]);

			glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
		}

		*/

		glBindVertexArray(0);
		//glDisable(GL_BLEND);
	}
}

void MeshRenderer::onDestroy()
{
	glDeleteVertexArrays(1, &_vao);
}
