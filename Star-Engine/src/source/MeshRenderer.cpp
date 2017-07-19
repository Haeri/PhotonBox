#include "../header/MeshRenderer.h"
#include "../header/Display.h"

void MeshRenderer::init()
{
	glGenVertexArrays(1, &_vao);

	glBindVertexArray(_vao);
	glGenBuffers(NUM_BUFFER, _vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[POS_BUFFR]);
	glBufferData(GL_ARRAY_BUFFER, _mesh->getVertexLength() * sizeof(Vertex), _mesh->getVertices(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void MeshRenderer::render()
{
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, _mesh->getVertexLength());
	glBindVertexArray(0);
}

void MeshRenderer::onDestroy()
{
	glDeleteVertexArrays(1, &_vao);
}
