#include "PhotonBox/component/LightProbe.h"

#include "PhotonBox/core/system/Lighting.h"
#include "PhotonBox/core/system/Renderer.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"
#include "PhotonBox/resource/LightMap.h"
#include "PhotonBox/resource/CubeMap.h"
#include "PhotonBox/core/system/SceneManager.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void LightProbe::init()
{
	Lighting::addLightProbe(this);
}

void LightProbe::capture()
{
	_lightMap = captureRecursive(_steps);
}

LightMap* LightProbe::captureRecursive(int step)
{
	if (step == 0)
	{
		LightMap* DEF = new LightMap(std::vector<std::string> {
			Resources::ENGINE_RESOURCES + "/default_emission.png",
			Resources::ENGINE_RESOURCES + "/default_emission.png",
			Resources::ENGINE_RESOURCES + "/default_emission.png",
			Resources::ENGINE_RESOURCES + "/default_emission.png",
			Resources::ENGINE_RESOURCES + "/default_emission.png",
			Resources::ENGINE_RESOURCES + "/default_emission.png",
		});
		return DEF;
	}

	LightMap* lastLightMap = captureRecursive(--step);

	//lightMap->enviromentMap = 
	CubeMap* current = new CubeMap(resolution);
	LightMap* lightMap = new LightMap(current, false);

	Camera* oldMain = Camera::getMainCamera();

	Entity* e_cam = SceneManager::getCurrentScene()->instantiate("tempCam");
	Camera* c_cam = e_cam->addComponent<Camera>();
	c_cam->setPerspectiveProjection(90.0f, 1.0f, 0.01f, 10.0f);
	c_cam->setMain();

	Matrix4f captureProjection = Matrix4f::createPerspective(90.0f, 1.0f, 0.01f, 10.0f);

	float PI = 3.1415f;
	Vector3f rotations[6] = {
		Vector3f(0, -PI / 2.0f, PI),
		Vector3f(0,  PI / 2.0f, PI),
		Vector3f(PI / 2.0f, 0, PI),
		Vector3f(-PI / 2.0f, 0, PI),
		Vector3f(0, 0, PI),
		Vector3f(0, PI, PI),
	};

	c_cam->getTransform()->setPosition(transform->getPosition());

	GLuint _captureFBO;
	GLuint _captureRBO;

	glGenFramebuffers(1, &_captureFBO);
	glGenRenderbuffers(1, &_captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, _captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _captureRBO);

	Renderer::renderShadows();

	for (unsigned int i = 0; i < 6; ++i)
	{

		glViewport(0, 0, resolution, resolution);
		glBindFramebuffer(GL_FRAMEBUFFER, _captureFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, current->getLocation(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		c_cam->getTransform()->setRotation(rotations[i]);
		Renderer::captureScene(lastLightMap);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	oldMain->setMain();

	lightMap->generateLightMaps();

	delete current;
	delete lastLightMap;

	return lightMap;
}

CubeMap* LightProbe::getEnviromentCube()
{
	return _lightMap->getEnviromentMap();
}

CubeMap* LightProbe::getIrradianceCube()
{
	return _lightMap->getIrradianceMap(); 
}

CubeMap* LightProbe::getSpecularCube()
{
	return _lightMap->getSpecularConvolutionMap(); 
}

void LightProbe::destroy()
{
	delete _lightMap;
	Lighting::removeFromLightProbeList(this);
}
