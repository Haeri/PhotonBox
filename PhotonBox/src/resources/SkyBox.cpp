#include "PhotonBox/resources/SkyBox.h"

#include "PhotonBox/components/Camera.h"
#include "PhotonBox/core/LightMap.h"
#include "PhotonBox/core/systems/Renderer.h"
#include "PhotonBox/core/systems/SceneManager.h"
#include "PhotonBox/resources/Resources.h"
#include "PhotonBox/resources/OBJLoader.h"
#include "PhotonBox/resources/Scene.h"
#include "PhotonBox/resources/SkyBoxShader.h"
#include "PhotonBox/resources/Mesh.h"

#ifdef MEM_DEBUG
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
	_mesh = SceneManager::getCurrentScene()->createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/skyBox.obj", true);
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
