#include "../header/MeshRenderer.h"
#include "../header/Display.h"
#include "../header/Transform.h"
#include "../header/Camera.h"

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

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

void MeshRenderer::render()
{
	_material->shader->bind();
	if(_material->texture != nullptr)
		_material->texture->bind();

	Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
	std::cout << mvp << std::endl;
	_material->shader->update(transform->getTransformationMatrix());
	//_material->shader->update(mvp);

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MeshRenderer::onDestroy()
{
	glDeleteVertexArrays(1, &_vao);
}
