#ifndef DEMO_SCENE_CPP
#define DEMO_SCENE_CPP

#include <Components/MeshRenderer.h>
#include <Components/PointRenderer.h>
#include <Resources/OBJLoader.h>
#include <Resources/Scene.h>
#include <Resources/Texture.h>
#include <Resources/Resources.h>
#include <resources/GShader.h>
#include <Resources/LitShader.h>

#include "../PostProcessors/SSAOProcessor.cpp"
#include "../PostProcessors/SSReflectionProcessor.cpp"
#include "../PostProcessors/ToneMappingProcessor.cpp"
#include "../PostProcessors/AutoExposureProcessor.cpp"
#include "../PostProcessors/BloomProcessor.cpp"
#include "../Scripts/CameraControllerScript.cpp"
#include "../Scripts/MaterialScript.cpp"

class DemoScene : public Scene
{
public:

	void Load()
	{

		/* --------------------------- RESOURCES --------------------------- */
		std::vector<std::string> nightSky = {
			Resources::ENGINE_RESOURCES + "/default_emission.png",
			Resources::ENGINE_RESOURCES + "/default_emission.png",
			Resources::ENGINE_RESOURCES + "/default_emission.png",
			Resources::ENGINE_RESOURCES + "/default_emission.png",
			Resources::ENGINE_RESOURCES + "/default_emission.png",
			Resources::ENGINE_RESOURCES + "/default_emission.png",
		};

		//Renderer::setSkyBox(createResource<CubeMap>(nightSky));
		Renderer::setSkyBox(new CubeMap(nightSky));
		Renderer::getSkyBox()->intensity = 1;
		

		/* --------------------------- POST PROCESSING --------------------------- */
		SSAOProcessor* p_ssao = new SSAOProcessor(0);
		SSReflectionProcessor* p_ssreflection = new SSReflectionProcessor(0);
		AutoExposureProcessor* p_autoExposure = new AutoExposureProcessor(1);
		BloomProcessor* p_bloom = new BloomProcessor(2);
		ToneMappingProcessor* p_tonemapping = new ToneMappingProcessor(3);


		/* --------------------------- OBJ --------------------------- */
		Mesh* plane = createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/plane.obj");
		Mesh* sphere = createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/sphere.obj");


		/* --------------------------- TEXTURES --------------------------- */
		Texture* default_normal = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_normal.png"), false);
		Texture* default_specular = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_specular.png"), false);
		Texture* default_emission = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_emission.png"), false);
		Texture* default_ao = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_ao.png"), false);
		Texture* default_roughness = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_roughness.png"), false);
		Texture* gradient = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/gradient.png"), false);

		/* --------------------------- SHADERS --------------------------- */
		GShader* defaultShader = GShader::getInstance();
		LitShader* litShader = LitShader::getInstance();


		/* --------------------------- MATERIALS --------------------------- */
		Material* def = createResource<Material>(defaultShader);
		def->setTexture("albedoMap", default_ao);
		def->setTexture("normalMap", default_normal);
		def->setTexture("roughnessMap", default_specular);
		def->setTexture("aoMap", default_ao);
		def->setTexture("metallicMap", default_emission);
		def->setTexture("emissionMap", default_emission);

		Material* lit = createResource<Material>(litShader);


		/* --------------------------- CAMERA --------------------------- */
		Entity* cam = instanciate("Camera");
		cam->addComponent<Camera>();
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 1, -6));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<CameraControllerScript>();



		/* --------------------------- LIGHTS --------------------------- */
		Entity* ambient = instanciate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(0.17f, 0.17f, 0.17f);
		ambient->getComponent<AmbientLight>()->intensity = 0.4f;
		//ambient->getComponent<AmbientLight>()->intensity = 0;


		Entity* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.93f, 0.92f, 0.94f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(1, -1, 1);
		sun->getComponent<DirectionalLight>()->intensity = 10.0f;
		sun->setEnable(false);


		Entity* pointLight = instanciate("Pointlight");
		//pointLight->addComponent<PointRenderer>();
		pointLight->getComponent<Transform>()->setPosition(Vector3f(0, 2, -1));
		pointLight->addComponent<PointLight>();
		pointLight->getComponent<PointLight>()->color = Vector3f(0.1f, 0.92f, 0.1f);
		pointLight->getComponent<PointLight>()->constant = 2;
		pointLight->getComponent<PointLight>()->linear = 0.09f;
		pointLight->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight->getComponent<PointLight>()->intensity = 3.6f;
		//pointLight->setEnable(false);


		Entity* pointLight2 = instanciate("Pointlight2");
		pointLight2->addComponent<PointRenderer>()->setMaterial(lit);
		pointLight2->getComponent<Transform>()->setPosition(Vector3f(1.7f, 2.0f, -1.0f));
		pointLight2->addComponent<PointLight>();
		pointLight2->getComponent<PointLight>()->color = Vector3f(0.1f, 0.1f, 0.94f);
		pointLight2->getComponent<PointLight>()->constant = 2;
		pointLight2->getComponent<PointLight>()->linear = 0.09f;
		pointLight2->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight2->getComponent<PointLight>()->intensity = 3.6f;
		//pointLight2->setEnable(false);

		Entity* pointLight3 = instanciate("Pointlight3");
		pointLight3->addComponent<PointRenderer>()->setMaterial(lit);
		pointLight3->getComponent<Transform>()->setPosition(Vector3f(-1.7f, 2.0f, -1.0f));
		pointLight3->addComponent<PointLight>();
		pointLight3->getComponent<PointLight>()->color = Vector3f(0.93f, 0.1f, 0.1f);
		pointLight3->getComponent<PointLight>()->constant = 2;
		pointLight3->getComponent<PointLight>()->linear = 0.09f;
		pointLight3->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight3->getComponent<PointLight>()->intensity = 3.6f;
		//pointLight3->setEnable(false);

		Entity* spot = instanciate("Spot");
		spot->getComponent<Transform>()->setPosition(Vector3f(1, 7, -1));
		spot->getComponent<Transform>()->setRotation(Vector3f(-PI / 2.0f, 0.0f, -0.3f));
		spot->addComponent<SpotLight>();
		spot->getComponent<SpotLight>()->coneAngle = 0.97f;
		spot->getComponent<SpotLight>()->coneAttenuation = 0.96f;
		spot->getComponent<SpotLight>()->constant = 2.0f;
		spot->getComponent<SpotLight>()->linear = 0.09f;
		spot->getComponent<SpotLight>()->quadratic = 0.032f;
		spot->getComponent<SpotLight>()->color = Vector3f(0.97f, 0.96f, 0.98f);
		spot->getComponent<SpotLight>()->intensity = 3.6f;
		spot->setEnable(false);


		Entity* probe = instanciate("Probe-1");
		probe->getComponent<Transform>()->setPosition(Vector3f(0.0f, 1.0f, 0.0f));
		probe->addComponent<MeshRenderer>();
		probe->getComponent<MeshRenderer>()->setMesh(sphere);
		probe->getComponent<MeshRenderer>()->setMaterial(def);

		Entity* quad = instanciate("Quad-1");
		quad->getComponent<Transform>()->setPosition(Vector3f(0, 0, -3));
		quad->getComponent<Transform>()->setScale(Vector3f(20, 20, 20));
		quad->addComponent<MeshRenderer>();
		quad->getComponent<MeshRenderer>()->setMesh(plane);
		quad->getComponent<MeshRenderer>()->setMaterial(def);
	}

	void OnUnload() {}

};
#endif // DEMO_SCENE_CPP