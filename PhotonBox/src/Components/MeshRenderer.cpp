#include "../Core/GameObject.h"
#include "../Core/OpenGL.h"
#include "../Core/Systems/Lighting.h"
#include "../Core/Systems/Renderer.h"
#include "../Resources/SkyBox.h"
#include "../Resources/Texture.h"
#include "AmbientLight.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "MeshRenderer.h"
#include "PointLight.h"
#include "Transform.h"

void MeshRenderer::init()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, _mesh->vertices.size() * sizeof(Vertex), &(_mesh->vertices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh->indices.size() * sizeof(unsigned int), &(_mesh->indices[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(Vertex::AttibLocation::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(Vertex::AttibLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(Vertex::AttibLocation::COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(Vertex::AttibLocation::TEXTURECOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glVertexAttribPointer(Vertex::AttibLocation::TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshRenderer::render()
{
	render(nullptr, nullptr);
}

void MeshRenderer::render(Shader* shader)
{
	render(shader, nullptr);
}

void MeshRenderer::render(Shader* shader, LightEmitter* light)
{
	if (shader == nullptr) shader = _material->shader;

	glBindVertexArray(_vao);
	shader->bind();

	if (light == nullptr)
		shader->update(transform);
	else
		shader->update(transform, light);

	_material->updateUniforms(shader);
	_material->bindTextures(shader);

	shader->updateTextures();
	shader->enableAttributes();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
	Renderer::addDrawCall();
	shader->disableAttributes();

	glBindVertexArray(0);
}

void MeshRenderer::onDestroy()
{
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao);
}

float MeshRenderer::getBoundingSphereRadius()
{
	return _mesh->boundingSphereRadius;
}
