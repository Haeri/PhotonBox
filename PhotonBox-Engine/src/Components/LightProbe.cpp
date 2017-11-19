#include "LightProbe.h"
#include "../Core/Systems/Lighting.h"
#include "../Components/Camera.h"
#include "../Core/Systems/Renderer.h"

LightProbe::LightProbe(){
	Lighting::addLightProbe(this);
}

LightProbe::LightProbe(CubeMap* enviromentMap){
	_envCube = enviromentMap;
	generateLightMaps();
}

void LightProbe::capture() {
	_envCube = new CubeMap(resolution);

	Camera* oldMain = Camera::getMainCamera();
	Camera cam = Camera();
	cam.setProjection(90.0f, 1.0f, 0.01f, 10.0f);
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


	glViewport(0, 0, resolution, resolution);
	glBindFramebuffer(GL_FRAMEBUFFER, _captureFBO);

	for (unsigned int i = 0; i < 6; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _envCube->getLocation(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cam.transform->setRotation(rotations[i]);
		Renderer::render();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	oldMain->setMain();

	Renderer::setSkyBox(_envCube);

	//generateLightMaps();
}

void LightProbe::generateLightMaps(){
	_irrCube = new CubeMap(32);
	_irrCube->generateIrradiance(_envCube->getLocation());

	_specCube = new CubeMap(128, true);
	_specCube->generateSpecularConvolution(_envCube->getLocation());
}
