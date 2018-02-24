#include "../Components/Camera.h"
#include "../Core/Systems/Renderer.h"
#include "OBJLoader.h"
#include "SkyBox.h"


void SkyBox::setLightProbe(LightProbe * lightProbe)
{
	_lp = lightProbe;
}

void SkyBox::init()
{
	_skyBoxShader = SkyBoxShader::getInstance();
	_mesh = OBJLoader::loadObj("./res/primitives/skyBox.obj");
	genVAO();
}

/*
void SkyBox::setEnviromentMap(CubeMap* cubeMap) {
	_evMap = cubeMap;
}

void SkyBox::setIrradienceMap(CubeMap* cubeMap) {
	_irrMap = cubeMap;
}
*/

void SkyBox::genVAO()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, _mesh->vertices.size() * sizeof(Vertex), &(_mesh->vertices[0]), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh->indices.size() * sizeof(unsigned int), &(_mesh->indices[0]), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(Vertex::AttibLocation::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(Vertex::AttibLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(Vertex::AttibLocation::TEXTURECOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

void SkyBox::render()
{
	if (_lp == nullptr) return;

	Matrix4f vp = Camera::getMainCamera()->getViewMatrix();
	vp(3, 0) = 0;
	vp(3, 1) = 0;
	vp(3, 2) = 0;
	vp = Camera::getMainCamera()->getProjectionMatrix() * vp;
	glBindVertexArray(_vao);
	glDepthMask(GL_FALSE);

	_skyBoxShader->bind();
	_skyBoxShader->update(vp);
	_skyBoxShader->setUniform("intensity", intensity);
	_skyBoxShader->enableAttributes();
	_lp->getEnviromentCube()->bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	Renderer::addDrawCall();
	_skyBoxShader->disableAttributes();

	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
}
