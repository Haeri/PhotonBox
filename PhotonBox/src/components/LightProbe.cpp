#include "PhotonBox/components/LightProbe.h"

#include "PhotonBox/core/systems/Lighting.h"
#include "PhotonBox/core/systems/Renderer.h"
#include "PhotonBox/resources/Resources.h"
#include "PhotonBox/components/Camera.h"
#include "PhotonBox/core/LightMap.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

LightProbe::LightProbe(int steps)
	:_steps(steps)
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

	LightMap* lightMap = new LightMap();
	lightMap->enviromentMap = new CubeMap(resolution);

	Camera* oldMain = Camera::getMainCamera();
	Camera cam = Camera();
	cam.setPerspectiveProjection(90.0f, 1.0f, 0.01f, 10.0f);
	cam.setMain();

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

	Transform* _transform = new Transform();
	cam.transform = _transform;
	cam.transform->setPosition(transform->getPosition());

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
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, lightMap->enviromentMap->getLocation(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cam.transform->setRotation(rotations[i]);
		Renderer::captureScene(lastLightMap);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	oldMain->setMain();

	lightMap->generateLightMaps();

	delete lastLightMap;
	delete _transform;

	return lightMap;
}

CubeMap* LightProbe::getEnviromentCube()
{
	return _lightMap->enviromentMap;
}

CubeMap* LightProbe::getIrradianceCube()
{
	return _lightMap->irradianceMap; 
}

CubeMap* LightProbe::getSpecularCube()
{
	return _lightMap->specularMap; 
}

void LightProbe::destroy()
{
	delete _lightMap;
	Lighting::removeFromLightProbeList(this);
}
