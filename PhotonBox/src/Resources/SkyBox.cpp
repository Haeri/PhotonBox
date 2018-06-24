#include "PhotonBox/resources/SkyBox.h"

#include "PhotonBox/components/Camera.h"
#include "PhotonBox/core/systems/Renderer.h"
#include "PhotonBox/resources/Resources.h"
#include "PhotonBox/resources/OBJLoader.h"
#include "PhotonBox/core/systems/SceneManager.h"
#include "PhotonBox/resources/Scene.h"
#include "PhotonBox/resources/SkyBoxShader.h"
#include "PhotonBox/resources/Mesh.h"
#include "PhotonBox/core/LightMap.h"

GLuint SkyBox::_vao = -1, SkyBox::_vbo, SkyBox::_ebo;
Mesh* SkyBox::_mesh;

SkyBox::~SkyBox()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);

	delete _lightMap;
	_vao = -1;
}

void SkyBox::init()
{
	_skyBoxShader = SkyBoxShader::getInstance();
	_mesh = SceneManager::getCurrentScene()->createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/skyBox.obj");
	genVAO();
}

void SkyBox::genVAO()
{
	if (_vao != -1) return;

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
}

void SkyBox::setCubeMap(CubeMap cubeMap)
{
	_lightMap = new LightMap(cubeMap);
}

void SkyBox::setColor(Vector3f color)
{
	_color = color;
}

void SkyBox::setDrawMode(SkyBoxMode mode)
{
	_mode = mode;
}

LightMap* SkyBox::getLightMap()
{
	return _lightMap;
}

void SkyBox::render()
{
	if (_lightMap == nullptr) return;

	//TODO: If skyBox set, else solid color.
	Matrix4f vp = Camera::getMainCamera()->getViewMatrix();
	vp(3, 0) = 0;
	vp(3, 1) = 0;
	vp(3, 2) = 0;
	vp = Camera::getMainCamera()->getProjectionMatrix() * vp;
	glBindVertexArray(_vao);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	_skyBoxShader->bind();
	_skyBoxShader->update(vp);
	_skyBoxShader->setUniform("intensity", intensity);
	_skyBoxShader->enableAttributes();
	_lightMap->enviromentMap->bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	Renderer::addDrawCall();
	_skyBoxShader->disableAttributes();

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glBindVertexArray(0);
}

void SkyBox::reset()
{
	delete _lightMap;
}