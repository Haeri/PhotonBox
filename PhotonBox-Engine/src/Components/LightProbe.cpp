#include "LightProbe.h"
#include "../Core/Systems/Lighting.h"
#include "../Components/Camera.h"
#include "../Core/Systems/Renderer.h"
#include "../Core/FrameBuffer.h"
#include "../Resources/DefaultPostShader.h"

LightProbe::LightProbe()
{
	Lighting::addLightProbe(this);
}

LightProbe::LightProbe(CubeMap* enviromentMap)
{
	_lightMap.enviromentMap = enviromentMap;
	_lightMap.generate();
}

void LightProbe::generateLightMap()
{
	// create a vector of LightMaps
	const int stepSize = 4;
	LightMap* lightMaps[stepSize];

	lightMaps[0] = new LightMap(new CubeMap(std::vector<std::string> {
		"./res/default_emission.png",
			"./res/default_emission.png",
			"./res/default_emission.png",
			"./res/default_emission.png",
			"./res/default_emission.png",
			"./res/default_emission.png",
	}));



	//loop cnt times
	for (size_t i = 0; i < stepSize - 1; ++i)
	{
		//capture scene
		lightMaps[i + 1] = captureAmbient(i, lightMaps[i]);
	}

	_lightMap = *lightMaps[3];

	/*
	for (size_t i = 0; i < 6; i++)
	{
		FrameBuffer::resetDefaultBuffer();
		Display::clearBuffers();


		/// TEMP CODE
		glBindVertexArray(FrameBuffer::_quadVAO);

		Shader* shader = DefaultPostShader::getInstance();
		shader->bind();
		shader->update(nullptr);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, lightMaps[i]->enviromentMap->getLocation());

		shader->updateTextures();
		shader->enableAttributes();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		shader->disableAttributes();

		glBindVertexArray(0);

		Display::swapBuffer();
	}
	*/

	//combine all lightmaps
	//Display::swapBuffer();
}

LightMap* LightProbe::captureAmbient(int pass, LightMap* lastLightMap)
{
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

	if (pass == 0)
		Renderer::renderShadows(false);

	glGenFramebuffers(1, &_captureFBO);
	glGenRenderbuffers(1, &_captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, _captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _captureRBO);


	for (unsigned int i = 0; i < 6; ++i)
	{

		glViewport(0, 0, resolution, resolution);
		glBindFramebuffer(GL_FRAMEBUFFER, _captureFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, lightMap->enviromentMap->getLocation(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cam.transform->setRotation(rotations[i]);
		Renderer::renderAmbient(pass, lastLightMap, &bounds);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	oldMain->setMain();

	lightMap->generate();

	return lightMap;
}

void LightProbe::capture()
{
	_lightMap = *captureRecursive(2);
}

LightMap* LightProbe::captureRecursive(int step)
{
	if (step == 0)
	{
		LightMap* DEF = new LightMap(new CubeMap(
			std::vector<std::string> {
			"./res/default_emission.png",
				"./res/default_emission.png",
				"./res/default_emission.png",
				"./res/default_emission.png",
				"./res/default_emission.png",
				"./res/default_emission.png",
		}));
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


	for (unsigned int i = 0; i < 6; ++i)
	{
		Renderer::renderShadows(true);

		glViewport(0, 0, resolution, resolution);
		glBindFramebuffer(GL_FRAMEBUFFER, _captureFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, lightMap->enviromentMap->getLocation(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cam.transform->setRotation(rotations[i]);
		Renderer::render(true, lastLightMap);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	oldMain->setMain();

	lightMap->generate();

	delete lastLightMap;
	return lightMap;
}

void LightProbe::destroy()
{
	Lighting::removeFromLightProbeList(this);
}
