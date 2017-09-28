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
#include "../Resources/SkyBox.h"
#include "../Resources/Texture.h"

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
	default_emission = new Texture("./res/default_emission.png", false);
	default_ao = new Texture("./res/default_ao.png", false);
}

void MeshRenderer::render() {
	render(nullptr, nullptr);
}

void MeshRenderer::render(Shader* shader) {
	render(shader, nullptr);
}

void MeshRenderer::render(Shader* shader, LightEmitter* light)
{
	if (shader == nullptr) shader = _material->shader;

	glBindVertexArray(_vao);
	shader->bind();

	if(light == nullptr)
		shader->update(transform);
	else
		shader->update(transform, light);

	_material->updateUniforms();
	_material->updateTextures();
	shader->updateTextures();
	shader->enableAttributes();
	glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
	shader->enableAttributes();		
		
		
	glBindVertexArray(0);
}

void MeshRenderer::onDestroy()
{
	glDeleteVertexArrays(1, &_vao);
}
