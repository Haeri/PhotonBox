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
		if (_material->shader->attributes["position"] != -1) {
			glEnableVertexAttribArray(_material->shader->attributes["position"]);
			glVertexAttribPointer(_material->shader->attributes["position"], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		}

		// vertex normals
		if (_material->shader->attributes["normal"] != -1) {
			glEnableVertexAttribArray(_material->shader->attributes["normal"]);
			glVertexAttribPointer(_material->shader->attributes["normal"], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		}

		// vertex colors
		if (_material->shader->attributes["color"] != -1) {
			glEnableVertexAttribArray(_material->shader->attributes["color"]);
			glVertexAttribPointer(_material->shader->attributes["color"], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		}

		// vertex texture coords
		if (_material->shader->attributes["uv"] != -1) {
			glEnableVertexAttribArray(_material->shader->attributes["uv"]);
			glVertexAttribPointer(_material->shader->attributes["uv"], 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		}
	}else if (Renderer::renderMode == Renderer::RenderMode::FORWARD) {
		// vertex positions
		if (_material->forwardShader->ambientShader->attributes["position"] != -1) {
			glEnableVertexAttribArray(_material->forwardShader->ambientShader->attributes["position"]);
			glVertexAttribPointer(_material->forwardShader->ambientShader->attributes["position"], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		}

		// vertex normals
		if (_material->forwardShader->ambientShader->attributes["normal"] != -1) {
			glEnableVertexAttribArray(_material->forwardShader->ambientShader->attributes["normal"]);
			glVertexAttribPointer(_material->forwardShader->ambientShader->attributes["normal"], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		}

		// vertex colors
		if (_material->forwardShader->ambientShader->attributes["color"] != -1) {
			glEnableVertexAttribArray(_material->forwardShader->ambientShader->attributes["color"]);
			glVertexAttribPointer(_material->forwardShader->ambientShader->attributes["color"], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		}

		// vertex texture coords
		if (_material->forwardShader->ambientShader->attributes["uv"] != -1) {
			glEnableVertexAttribArray(_material->forwardShader->ambientShader->attributes["uv"]);
			glVertexAttribPointer(_material->forwardShader->ambientShader->attributes["uv"], 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		}







		// vertex positions
		if (_material->forwardShader->pointLightShader->attributes["position"] != -1) {
			glEnableVertexAttribArray(_material->forwardShader->pointLightShader->attributes["position"]);
			glVertexAttribPointer(_material->forwardShader->pointLightShader->attributes["position"], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		}

		// vertex normals
		if (_material->forwardShader->pointLightShader->attributes["normal"] != -1) {
			glEnableVertexAttribArray(_material->forwardShader->pointLightShader->attributes["normal"]);
			glVertexAttribPointer(_material->forwardShader->pointLightShader->attributes["normal"], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		}

		// vertex colors
		if (_material->forwardShader->pointLightShader->attributes["color"] != -1) {
			glEnableVertexAttribArray(_material->forwardShader->pointLightShader->attributes["color"]);
			glVertexAttribPointer(_material->forwardShader->pointLightShader->attributes["color"], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		}

		// vertex texture coords
		if (_material->forwardShader->pointLightShader->attributes["uv"] != -1) {
			glEnableVertexAttribArray(_material->forwardShader->pointLightShader->attributes["uv"]);
			glVertexAttribPointer(_material->forwardShader->pointLightShader->attributes["uv"], 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
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
	
		
		glBindVertexArray(_vao);


		if (_material->texture != nullptr)
			_material->texture->bind();

		

		// AMBIENT
		//LightEmitter* ambient = Lighting::getLights<LightEmitter>()[0];
		_material->forwardShader->bindAmbientShader();

		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		_material->forwardShader->ambientShader->update(mvp);

		glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
		



		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);

		
		
		// POINTLIGHTS
		std::vector<PointLight*> pointLights = Lighting::getLights<PointLight>();
		for(size_t i = 0; i < pointLights.size(); ++i){
			_material->forwardShader->bindPointLightShader();
			Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
			Vector4f eyePos = Camera::getMainCamera()->getViewProjection() * Vector4f(Camera::getMainCamera()->transform->getPositionWorld(), 1);
			_material->forwardShader->pointLightShader->update(mvp, *pointLights[i], eyePos);

			glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
		}

		
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
