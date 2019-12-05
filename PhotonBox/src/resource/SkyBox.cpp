#include "PhotonBox/resource/SkyBox.h"

#include "PhotonBox/component/Camera.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/core/system/Renderer.h"
#include "PhotonBox/core/system/SceneManager.h"
#include "PhotonBox/resource/LightMap.h"
#include "PhotonBox/resource/Scene.h"
#include "PhotonBox/resource/Mesh.h"
#include "PhotonBox/resource/shader/SkyBoxShader.h"
#include "PhotonBox/util/OBJLoader.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

Mesh* SkyBox::_mesh = nullptr;

SkyBox::~SkyBox()
{
	delete _lightMap;
}

void SkyBox::init()
{
	_skyBoxShader = SkyBoxShader::getInstance();
	_mesh = SceneManager::getCurrentScene()->createResource<Mesh>(Filepath(Resources::ENGINE_RESOURCES + "/primitives/skyBox.obj"), Mesh::Config{ true });
}

void SkyBox::setCubeMap(CubeMap* cubeMap)
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
	glBindVertexArray(_mesh->getVAO());
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	_skyBoxShader->bind();
	_skyBoxShader->update(vp);
	_skyBoxShader->setUniform("intensity", intensity);
	_skyBoxShader->enableAttributes();
	_lightMap->getEnviromentMap()->bind();
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
