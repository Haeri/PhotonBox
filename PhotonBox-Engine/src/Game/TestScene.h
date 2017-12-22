#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "../Resources/Scene.h"
#include "../Components/MeshRenderer.h"
#include "../Components/TransparentMeshRenderer.h"
#include "../Components/Camera.h"
#include "TransformerScript.cpp"
#include "../Resources/OBJLoader.h"
#include "../Components/PointLight.h"
#include "../Resources/BasicShader.h"
#include "../Resources/LitShader.h"
#include "PrinterScript.cpp"
#include "../Components/PointRenderer.h"
#include "CameraController.h"
#include "../Components/AmbientLight.h"
#include "../Game/MaterialScript.h"
#include "../Game/BlurShader.h"
#include "../Game/BloomProcessor.h"
#include "../Game/BlurProcessor.h"
#include "../Resources/ToneMappingProcessor.h"
#include "../Resources/IrradianceShader.h"
#include "../Resources/Texture.h"
#include "../Resources/AutoExposureProcessor.h"
#include "../Resources/TransparentShader.h"
#include "../Components/SpotLight.h"
#include "../Game/PathWalkerScript.h"
#include "../Game/ControllerToggleScript.h"
#include "../Resources/SkyBoost.h"
#include "../Resources/SSAOProcessor.h"

#define PI 3.14159265359

class TestScene : public Scene {
public:
	CubeMap* sky;

	Mesh* plane;
	Mesh* couchMesh;
	Mesh* wallMesh;
	Mesh* floorMesh;

	Texture* woodAlbedo;
	Texture* woodRough;
	Texture* woodNormal;
	Texture* woodAo;
	Texture* woodMetal;

	Texture* couchAlbedo;
	Texture* couchNormal;
	Texture* couchRoughness;
	Texture* couchAo;

	Texture* wallAlbedo;
	Texture* wallNormal;

	Texture* default_normal;
	Texture* default_specular;
	Texture* default_emission;
	Texture* default_ao;
	Texture* gradient;

	Material* wood;
	Material* def;
	Material* couchMaterial;
	Material* wallMaterial;

	void Load() {

		/* --------------------------- RESOURCES --------------------------- */
		std::vector<std::string> skyBoxLod = {
			"./res/enviroment/lod0_right.jpg",
			"./res/enviroment/lod0_left.jpg",
			"./res/enviroment/lod0_top.jpg",
			"./res/enviroment/lod0_bottom.jpg",
			"./res/enviroment/lod0_back.jpg",
			"./res/enviroment/lod0_front.jpg",
		};
		std::vector<std::string> skyBoxLod2 = {
			"./res/dark/posx.jpg",
			"./res/dark/negx.jpg",
			"./res/dark/posy.jpg",
			"./res/dark/negy.jpg",
			"./res/dark/posz.jpg",
			"./res/dark/negz.jpg",
		};
		std::vector<std::string> skyBoxLod3 = {
			"./res/redGreenRoom/lod0_right.jpg",
			"./res/redGreenRoom/lod0_left.jpg",
			"./res/redGreenRoom/lod0_top.jpg",
			"./res/redGreenRoom/lod0_bottom.jpg",
			"./res/redGreenRoom/lod0_back.jpg",
			"./res/redGreenRoom/lod0_front.jpg",
		};

		std::vector<std::string> skyBoxLod4 = {
			"./res/Def_Indoor/rest.png",
			"./res/Def_Indoor/rest.png",
			"./res/Def_Indoor/rest.png",
			"./res/Def_Indoor/rest.png",
			"./res/Def_Indoor/rest.png",
			"./res/Def_Indoor/rest.png",
		};

		std::vector<std::string> black = {
			"./res/default_roughness.png",
			"./res/default_roughness.png",
			"./res/default_roughness.png",
			"./res/default_roughness.png",
			"./res/default_roughness.png",
			"./res/default_roughness.png",
		};

		std::vector<std::string> white = {
			"./res/default_ao.png",
			"./res/default_ao.png",
			"./res/default_ao.png",
			"./res/default_ao.png",
			"./res/default_ao.png",
			"./res/default_ao.png",
		};

		Renderer::setSkyBox(new CubeMap(white));


		/* --------------------------- POST PROCESSING --------------------------- */
		AutoExposureProcessor* p_autoExposure = new AutoExposureProcessor(1);
		BloomProcessor* p_bloom = new BloomProcessor(2);
		ToneMappingProcessor* p_tonemapping = new ToneMappingProcessor(3);
		SSAOProcessor* p_ssao = new SSAOProcessor(0);


		/* --------------------------- OBJ --------------------------- */
		Mesh* sphere = OBJLoader::loadObj("./res/sphere.obj");
		plane = OBJLoader::loadObj("./res/plane.obj");
		couchMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Couch/couch.obj");
		wallMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Walls/Room.obj");
		floorMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Floor/Floor.obj");
		Mesh* doorsMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Doors/Doors.obj");
		Mesh* tableMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Table/Table.obj");
		Mesh* longCouchMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Couch/Long_Couch.obj");
		Mesh* slidingDoorMesh = OBJLoader::loadObj("./res/Realistic-Rendering/SlidingDoor/SlideDoor.obj");
		Mesh* floorEdgeMesh = OBJLoader::loadObj("./res/Realistic-Rendering/FloorEdge/FloorEdge.obj");
		Mesh* shelveMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Shelving/Shelving.obj");
		Mesh* carpetMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Carpet/Carpet.obj");
		Mesh* carpetFloorMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Carpet/CarpetFloor.obj");
		Mesh* occluderMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Occluder/occluder.obj");
		Mesh* windowMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Window/Window.obj");
		Mesh* panoramaMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Panorama/Panorama.obj");
		Mesh* lampMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Lamp/Lamp.obj");
		Mesh* ceilingLightMesh = OBJLoader::loadObj("./res/Realistic-Rendering/CeilingLight/CeilingLight.obj");
		Mesh* vaseMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Vase/Vase.obj");
		Mesh* bookMesh1 = OBJLoader::loadObj("./res/Realistic-Rendering/Books/Books_1.obj");
		Mesh* bookMesh2 = OBJLoader::loadObj("./res/Realistic-Rendering/Books/Books_2.obj");
		Mesh* bookMesh3 = OBJLoader::loadObj("./res/Realistic-Rendering/Books/Books_3.obj");
		Mesh* bookMesh4 = OBJLoader::loadObj("./res/Realistic-Rendering/Books/Books_4.obj");
		Mesh* balconyFloorMesh = OBJLoader::loadObj("./res/Realistic-Rendering/BalconyFloor/Balcony_Floor.obj");
		Mesh* balconyRailingMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Railing/Balcony_Railing.obj");


		/* --------------------------- TEXTURES --------------------------- */
		woodAlbedo = new Texture("./res/materials/mahogfloor/mahogfloor_basecolor.png", true);
		woodRough = new Texture("./res/materials/mahogfloor/mahogfloor_roughness.png", true);
		woodNormal = new Texture("./res/materials/mahogfloor/mahogfloor_normal.png", true);
		woodAo = new Texture("./res/materials/mahogfloor/mahogfloor_AO.png", true);
		woodMetal = new Texture("./res/materials/mahogfloor/mahogfloor_metalness.png", true);

		Texture* doorAlbedo = new Texture("./res/Realistic-Rendering/Doors/T_Door_D.tga", true);
		Texture* doorRough = new Texture("./res/Realistic-Rendering/Doors/T_Door_R.tga", true);
		Texture* doorNormal = new Texture("./res/Realistic-Rendering/Doors/T_Door_N.TGA", true);
		Texture* doorAo = new Texture("./res/Realistic-Rendering/Doors/T_Door_AOMask.TGA", true);
		Texture* doorMetal = new Texture("./res/Realistic-Rendering/Doors/T_Door_M.tga", true);

		Texture* tableAlbedo = new Texture("./res/Realistic-Rendering/Table/T_Table_D.tga", true);
		Texture* tableRough = new Texture("./res/Realistic-Rendering/Table/T_Table_R.tga", true);
		Texture* tableNormal = new Texture("./res/Realistic-Rendering/Table/T_Table_N.TGA", true);
		Texture* tableAo = new Texture("./res/Realistic-Rendering/Table/T_Table_AOMASK.TGA", true);
		Texture* tableMetal = new Texture("./res/Realistic-Rendering/Table/T_Table_M.tga", true);

		//Texture* slidingDoorAlbedo = new Texture("./res/Realistic-Rendering/SlidingDoor/T_Table_D.tga", true);
		Texture* slidingDoorRough = new Texture("./res/Realistic-Rendering/SlidingDoor/T_SlidingDoor_R.tga", true);
		Texture* slidingDoorNormal = new Texture("./res/Realistic-Rendering/SlidingDoor/T_SlidingDoor_N.TGA", true);
		Texture* slidingDoorAo = new Texture("./res/Realistic-Rendering/SlidingDoor/T_SlidingDoor_AOMask.TGA", true);
		Texture* slidingDoorMetal = new Texture("./res/Realistic-Rendering/SlidingDoor/T_SlidingDoor_M.TGA", true);

		Texture* shelveAlbedo = new Texture("./res/Realistic-Rendering/Shelving/T_Shelving_D.TGA", true);
		Texture* shelveRough = new Texture("./res/Realistic-Rendering/Shelving/T_Shelving_R.TGA", true);
		Texture* shelveNormal = new Texture("./res/Realistic-Rendering/Shelving/T_Shelving_N.TGA", true);
		Texture* shelveAo = new Texture("./res/Realistic-Rendering/Shelving/T_Shelving_AO.TGA", true);

		Texture* carpetAlbedo = new Texture("./res/Realistic-Rendering/Carpet/T_Carpet_D.png", true);
		Texture* carpetNormal = new Texture("./res/Realistic-Rendering/Carpet/T_Carpet_N.TGA", true);
		Texture* carpetAo = new Texture("./res/Realistic-Rendering/Carpet/T_Carpet_AO.TGA", true);
		
		Texture* lampAlbedo = new Texture("./res/Realistic-Rendering/Lamp/T_FloorLamp_Albedo.TGA", true);
		Texture* lampRough = new Texture("./res/Realistic-Rendering/Lamp/T_FloorLamp_Roughness.TGA", true);
		Texture* lampNormal = new Texture("./res/Realistic-Rendering/Lamp/T_FloorLamp_N.TGA", true);
		Texture* lampAo = new Texture("./res/Realistic-Rendering/Lamp/T_FloorLamp_AO.TGA", true);
		Texture* lampMetal = new Texture("./res/Realistic-Rendering/Lamp/T_FloorLamp_M.TGA", true);

		Texture* ceilingAlbedo = new Texture("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_Albedo.TGA", true);
		Texture* ceilingRough = new Texture("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_Roughness.TGA", true);
		Texture* ceilingNormal = new Texture("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_N.TGA", true);
		Texture* ceilingAo = new Texture("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_AO.TGA", true);
		Texture* ceilingMetal = new Texture("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_M.TGA", true);

		Texture* vaseAlbedo = new Texture("./res/Realistic-Rendering/Vase/Vase_Albedo.png", true);
		Texture* vaseRough = new Texture("./res/Realistic-Rendering/Vase/Vase_Roughness.png", true);

		Texture* bookAo = new Texture("./res/Realistic-Rendering/Books/book_occlusion.tga", true);
		Texture* bookNormal = new Texture("./res/Realistic-Rendering/Books/book_normals.tga", true);
		Texture* bookAlbedo1 = new Texture("./res/Realistic-Rendering/Books/book_albedo_1.tga", true);
		Texture* bookAlbedo2 = new Texture("./res/Realistic-Rendering/Books/book_albedo_2.tga", true);
		Texture* bookAlbedo3 = new Texture("./res/Realistic-Rendering/Books/book_albedo_3.tga", true);
		Texture* bookAlbedo4 = new Texture("./res/Realistic-Rendering/Books/book_albedo_4.tga", true);

		Texture* balconyFloorAlbedo = new Texture("./res/Realistic-Rendering/BalconyFloor/T_StoneMix_D.tga", true);
		Texture* balconyFloorNormal = new Texture("./res/Realistic-Rendering/BalconyFloor/T_StoneMix_N.tga", true);
		Texture* balconyFloorRoughness = new Texture("./res/Realistic-Rendering/BalconyFloor/T_StoneMix_R.tga", true);

		Texture* galvanizedAlbedo = new Texture("./res/Realistic-Rendering/Railing/T_Galvanized_D.tga", true);
		Texture* galvanizedRough = new Texture("./res/Realistic-Rendering/Railing/T_Galvanized_Roughness.tga", true);
		Texture* galvanizedNormal = new Texture("./res/Realistic-Rendering/Railing/T_Galvanized_N.tga", true);
		Texture* railingAo = new Texture("./res/Realistic-Rendering/Railing/T_Railing_AOMASK.tga", true);
		Texture* railingNormal = new Texture("./res/Realistic-Rendering/Railing/T_Railing_N.tga", true);

		Texture* windowAlbedo = new Texture("./res/Realistic-Rendering/Window/albedo.png", true);

		couchAlbedo = new Texture("./res/Realistic-Rendering/Couch/T_Couch_D.TGA", true);
		couchNormal = new Texture("./res/Realistic-Rendering/Couch/T_Couch_N.TGA", true);
		couchRoughness = new Texture("./res/Realistic-Rendering/Couch/T_Couch_R.tga", true);
		couchAo = new Texture("./res/Realistic-Rendering/Couch/T_Couch_AO.TGA", true);
		Texture* couchMetal = new Texture("./res/Realistic-Rendering/Couch/T_Couch_M.TGA", true);

		wallNormal = new Texture("./res/Realistic-Rendering/Walls/T_PaintedWall_N.TGA", true);


		default_normal = new Texture("./res/default_normal.png", false);
		default_specular = new Texture("./res/default_specular.png", false);
		default_emission = new Texture("./res/default_emission.png", false);
		default_ao = new Texture("./res/default_ao.png", false);
		Texture* default_roughness = new Texture("./res/default_roughness.png", false);
		gradient = new Texture("./res/gri1.png", false);


		
		Texture* panoramaAlbedo = new Texture("./res/Realistic-Rendering/Panorama/T_Background_D.TGA", true);


		/* --------------------------- SHADERS --------------------------- */
		LitShader* litShader = LitShader::getInstance();
		TransparentShader* transparentShader = TransparentShader::getInstance();
		SkyBoostShader* skyBoost = SkyBoostShader::getInstance();


		/* --------------------------- MATERIALS --------------------------- */

		couchMaterial = new Material();
		couchMaterial->setTexture("albedoMap", couchAlbedo);
		couchMaterial->setTexture("normalMap", couchNormal);
		couchMaterial->setTexture("roughnessMap", couchRoughness);
		couchMaterial->setTexture("aoMap", couchAo);
		couchMaterial->setTexture("metallicMap", couchMetal);
		couchMaterial->setTexture("emissionMap", default_emission);

		wallMaterial = new Material();
		wallMaterial->setTexture("albedoMap", default_ao);
		wallMaterial->setTexture("normalMap", wallNormal);
		wallMaterial->setTexture("roughnessMap", default_ao);
		wallMaterial->setTexture("aoMap", default_ao);
		wallMaterial->setTexture("metallicMap", default_emission);
		wallMaterial->setTexture("emissionMap", default_emission);

		wood = new Material();
		wood->setTexture("albedoMap", woodAlbedo);
		wood->setTexture("normalMap", woodNormal);
		wood->setTexture("roughnessMap", woodRough);
		wood->setTexture("aoMap", woodAo);
		wood->setTexture("metallicMap", woodMetal);
		wood->setTexture("emissionMap", default_emission);

		Material* doorMaterial = new Material();
		doorMaterial->setTexture("albedoMap", doorAlbedo);
		doorMaterial->setTexture("normalMap", doorNormal);
		doorMaterial->setTexture("roughnessMap", doorRough);
		doorMaterial->setTexture("aoMap", doorAo);
		doorMaterial->setTexture("metallicMap", doorMetal);
		doorMaterial->setTexture("emissionMap", default_emission);

		Material* tableMaterial = new Material();
		tableMaterial->setTexture("albedoMap", tableAlbedo);
		tableMaterial->setTexture("normalMap", tableNormal);
		tableMaterial->setTexture("roughnessMap", tableRough);
		tableMaterial->setTexture("aoMap", tableAo);
		tableMaterial->setTexture("metallicMap", tableMetal);
		tableMaterial->setTexture("emissionMap", default_emission);

		Material* shelveMaterial = new Material();
		shelveMaterial->setTexture("albedoMap", shelveAlbedo);
		shelveMaterial->setTexture("normalMap", shelveNormal);
		shelveMaterial->setTexture("roughnessMap", shelveRough);
		shelveMaterial->setTexture("aoMap", shelveAo);
		shelveMaterial->setTexture("metallicMap", default_emission);
		shelveMaterial->setTexture("emissionMap", default_emission);

		Material* slidingDoorMaterial = new Material();
		slidingDoorMaterial->setTexture("albedoMap", default_ao);
		slidingDoorMaterial->setTexture("normalMap", slidingDoorNormal);
		slidingDoorMaterial->setTexture("roughnessMap", slidingDoorRough);
		slidingDoorMaterial->setTexture("aoMap", slidingDoorAo);
		slidingDoorMaterial->setTexture("metallicMap", slidingDoorMetal);
		slidingDoorMaterial->setTexture("emissionMap", default_emission);

		Material* carpetMaterial = new Material();
		carpetMaterial->setTexture("albedoMap", carpetAlbedo);
		carpetMaterial->setTexture("normalMap", carpetNormal);
		carpetMaterial->setTexture("roughnessMap", default_ao);
		carpetMaterial->setTexture("aoMap", carpetAo);
		carpetMaterial->setTexture("metallicMap", default_emission);
		carpetMaterial->setTexture("emissionMap", default_emission);

		Material* carpetFloorMaterial = new Material();
		carpetFloorMaterial->setTexture("albedoMap", carpetAlbedo);
		carpetFloorMaterial->setTexture("normalMap", default_normal);
		carpetFloorMaterial->setTexture("roughnessMap", default_ao);
		carpetFloorMaterial->setTexture("aoMap", carpetAo);
		carpetFloorMaterial->setTexture("metallicMap", default_emission);
		carpetFloorMaterial->setTexture("emissionMap", default_emission);

		Material* lampMaterial = new Material();
		lampMaterial->setTexture("albedoMap", lampAlbedo);
		lampMaterial->setTexture("normalMap", lampNormal);
		lampMaterial->setTexture("roughnessMap", lampRough);
		lampMaterial->setTexture("aoMap", lampAo);
		lampMaterial->setTexture("metallicMap", lampMetal);
		lampMaterial->setTexture("emissionMap", default_emission);

		Material* ceilingLightMaterial = new Material();
		ceilingLightMaterial->setTexture("albedoMap", ceilingAlbedo);
		ceilingLightMaterial->setTexture("normalMap", ceilingNormal);
		ceilingLightMaterial->setTexture("roughnessMap", ceilingRough);
		ceilingLightMaterial->setTexture("aoMap", ceilingAo);
		ceilingLightMaterial->setTexture("metallicMap", ceilingMetal);
		ceilingLightMaterial->setTexture("emissionMap", default_emission);

		Material* vaseMaterial = new Material();
		vaseMaterial->setTexture("albedoMap", vaseAlbedo);
		vaseMaterial->setTexture("normalMap", default_normal);
		vaseMaterial->setTexture("roughnessMap", vaseRough);
		vaseMaterial->setTexture("aoMap", default_ao);
		vaseMaterial->setTexture("metallicMap", default_emission);
		vaseMaterial->setTexture("emissionMap", default_emission);

		Material* bookMaterial1 = new Material();
		bookMaterial1->setTexture("albedoMap", bookAlbedo1);
		bookMaterial1->setTexture("normalMap", bookNormal);
		bookMaterial1->setTexture("roughnessMap", default_ao);
		bookMaterial1->setTexture("aoMap", bookAo);
		bookMaterial1->setTexture("metallicMap", default_emission);
		bookMaterial1->setTexture("emissionMap", default_emission);
		Material* bookMaterial2 = new Material();
		bookMaterial2->setTexture("albedoMap", bookAlbedo2);
		bookMaterial2->setTexture("normalMap", bookNormal);
		bookMaterial2->setTexture("roughnessMap", default_ao);
		bookMaterial2->setTexture("aoMap", bookAo);
		bookMaterial2->setTexture("metallicMap", default_emission);
		bookMaterial2->setTexture("emissionMap", default_emission);
		Material* bookMaterial3 = new Material();
		bookMaterial3->setTexture("albedoMap", bookAlbedo3);
		bookMaterial3->setTexture("normalMap", bookNormal);
		bookMaterial3->setTexture("roughnessMap", default_ao);
		bookMaterial3->setTexture("aoMap", bookAo);
		bookMaterial3->setTexture("metallicMap", default_emission);
		bookMaterial3->setTexture("emissionMap", default_emission);
		Material* bookMaterial4 = new Material();
		bookMaterial4->setTexture("albedoMap", bookAlbedo4);
		bookMaterial4->setTexture("normalMap", bookNormal);
		bookMaterial4->setTexture("roughnessMap", default_ao);
		bookMaterial4->setTexture("aoMap", bookAo);
		bookMaterial4->setTexture("metallicMap", default_emission);
		bookMaterial4->setTexture("emissionMap", default_emission);

		Material* railingMaterial = new Material();
		railingMaterial->setTexture("albedoMap", galvanizedAlbedo);
		railingMaterial->setTexture("normalMap", railingNormal);
		railingMaterial->setTexture("roughnessMap", galvanizedRough);
		railingMaterial->setTexture("aoMap", railingAo);
		railingMaterial->setTexture("metallicMap", default_ao);
		railingMaterial->setTexture("emissionMap", default_emission);

		Material* balconyFloorMaterial = new Material();
		balconyFloorMaterial->setTexture("albedoMap", balconyFloorAlbedo);
		balconyFloorMaterial->setTexture("normalMap", balconyFloorNormal);
		balconyFloorMaterial->setTexture("roughnessMap", balconyFloorRoughness);
		balconyFloorMaterial->setTexture("aoMap", default_ao);
		balconyFloorMaterial->setTexture("metallicMap", default_emission);
		balconyFloorMaterial->setTexture("emissionMap", default_emission);



		def = new Material();
		def->setTexture("albedoMap", default_specular);
		def->setTexture("normalMap", default_normal);
		def->setTexture("roughnessMap", default_emission);
		def->setTexture("aoMap", default_ao);
		def->setTexture("metallicMap", default_ao);
		def->setTexture("emissionMap", default_emission);

		Material* glassMaterial = new Material(transparentShader);
		//glassMaterial->setProperty<Vector4f>("tint", Vector4f(0, 0.001, 0.1, 0.1));
		glassMaterial->setTexture("albedoMap", windowAlbedo);
		glassMaterial->setTexture("normalMap", default_normal);
		glassMaterial->setTexture("roughnessMap", default_roughness);
		glassMaterial->setTexture("aoMap", default_ao);
		glassMaterial->setTexture("metallicMap", default_emission);
		glassMaterial->setTexture("emissionMap", default_emission);


		Material* occluderMaterial = new Material(litShader);
		occluderMaterial->setProperty<Vector3f>("color", Vector3f(0, 0, 0));

		Material* panoramaMaterial = new Material(skyBoost);
		panoramaMaterial->setTexture("albedoMap", panoramaAlbedo);
		panoramaMaterial->setProperty("boost", 10.0f);


		/* --------------------------- CAMERA --------------------------- */
		GameObject* s1 = instanciate("start");
		s1->getComponent<Transform>()->setPosition(Vector3f(1.5f, 0.7f, -2.4));
		s1->getComponent<Transform>()->setRotation(Vector3f(-0.1f, 0.4f, 0.0));

		GameObject* f1 = instanciate("fin");
		f1->getComponent<Transform>()->setPosition(Vector3f(-1.8f, 1.1f, -2.9));
		f1->getComponent<Transform>()->setRotation(Vector3f(-0.1f, -0.3f, 0.0));
		
		GameObject* s2 = instanciate("start");
		s2->getComponent<Transform>()->setPosition(Vector3f(-1.708001, 0.232390, 0.078278));
		s2->getComponent<Transform>()->setRotation(Vector3f(0.028630, -1.570984, 0.000000));

		GameObject* f2 = instanciate("fin");
		f2->getComponent<Transform>()->setPosition(Vector3f(-0.610981, 0.718269, 0.078073));
		f2->getComponent<Transform>()->setRotation(Vector3f(0.028630, -1.570984, 0.000000));

		GameObject* s3 = instanciate("start");
		s3->getComponent<Transform>()->setPosition(Vector3f(-1.432103, 0.233059, 3.00573));
		s3->getComponent<Transform>()->setRotation(Vector3f(-0.910666, 0.317027, 0.00000));

		GameObject* f3 = instanciate("start");
		f3->getComponent<Transform>()->setPosition(Vector3f(-1.254720, 0.108683, -0.22882));
		f3->getComponent<Transform>()->setRotation(Vector3f(0.086667, 0.071351, 0.00000));

		GameObject* s4 = instanciate("start");
		s4->getComponent<Transform>()->setPosition(Vector3f(0.568587, 0.651397, 0.84028));
		s4->getComponent<Transform>()->setRotation(Vector3f(-0.613333, 3.154054, 0.00000));

		GameObject* f4 = instanciate("start");
		f4->getComponent<Transform>()->setPosition(Vector3f(-0.214201, 0.652349, 0.81502));
		f4->getComponent<Transform>()->setRotation(Vector3f(-0.402666, 3.269999, 0.00000));

		GameObject* s5 = instanciate("start");
		s5->getComponent<Transform>()->setPosition(Vector3f(1.163253, 2.849120, -0.82634));
		s5->getComponent<Transform>()->setRotation(Vector3f(-1.466666, 7.874601, 0.00000));

		GameObject* f5 = instanciate("start");
		f5->getComponent<Transform>()->setPosition(Vector3f(1.112261, 2.849120, 1.64630));
		f5->getComponent<Transform>()->setRotation(Vector3f(-1.466666, 7.874601, 0.00000));

		GameObject* s6 = instanciate("start");
		s6->getComponent<Transform>()->setPosition(Vector3f(-1.275343, 0.609712, 3.92936));
		s6->getComponent<Transform>()->setRotation(Vector3f(-0.406666, 2.218385, 0.00000));

		GameObject* f6 = instanciate("start");
		f6->getComponent<Transform>()->setPosition(Vector3f(-1.701402, 0.896463, 3.75409));
		f6->getComponent<Transform>()->setRotation(Vector3f(0.070667, 2.363519, 0.00000));

		GameObject* s7 = instanciate("start");
		s7->getComponent<Transform>()->setPosition(Vector3f(1.269536, 2.121249, 4.091350));
		s7->getComponent<Transform>()->setRotation(Vector3f(-0.606236, 2.979938, 0.000000));

		GameObject* f7 = instanciate("start");
		f7->getComponent<Transform>()->setPosition(Vector3f(0.237463, 1.906774, 3.826429));
		f7->getComponent<Transform>()->setRotation(Vector3f(-0.576903, 3.178587, 0.000000));

		GameObject* s8 = instanciate("start");
		s8->getComponent<Transform>()->setPosition(Vector3f(1.469624, 0.217904, 1.738431));
		s8->getComponent<Transform>()->setRotation(Vector3f(-0.024902, 5.696155, 0.000000));

		GameObject* f8 = instanciate("start");
		f8->getComponent<Transform>()->setPosition(Vector3f(1.428122, 1.054274, 1.739055));
		f8->getComponent<Transform>()->setRotation(Vector3f(-0.024902, 5.696155, 0.000000));

		
		GameObject* cam = instanciate("Camera");
		cam->addComponent<Camera>();
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 2, -2));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<CameraController>();
		
		cam->addComponent<PathWalkerScript>()->speed = 0.1f;
		cam->getComponent<PathWalkerScript>()->path.push_back(*(s1->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(f1->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(s2->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(f2->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(s3->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(f3->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(s4->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(f4->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(s5->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(f5->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(s6->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(f6->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(s7->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(f7->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(s8->transform));
		cam->getComponent<PathWalkerScript>()->path.push_back(*(f8->transform));

		cam->addComponent<ControllerToggleScript>();


		//cam->addComponent<MaterialScript>()->material = p_tonemapping->material;


		/* --------------------------- LIGHT HELPER --------------------------- */
		GameObject* lightProbe = instanciate("LightProbe");
		lightProbe->addComponent<LightProbe>()->resolution = 512;
		lightProbe->getComponent<Transform>()->setPosition(Vector3f(0, 1.2f, 0));
		lightProbe->getComponent<LightProbe>()->bounds.setMinBound(Vector3f(-2.1f, -1.3f, -3.4));
		lightProbe->getComponent<LightProbe>()->bounds.setMaxBound(Vector3f(2.1f, 2.0f, 6));

		GameObject* min = instanciate("min");
		min->getComponent<Transform>()->setPosition(Vector3f(-2.1f, -1.3f, -3.4));

		GameObject* max = instanciate("max");
		max->getComponent<Transform>()->setPosition(Vector3f(2.1f, 2.0f, 6));


		/* --------------------------- LIGHTS --------------------------- */
		GameObject* ambient = instanciate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(0.3f, 0.3f, 0.3f);
		ambient->getComponent<AmbientLight>()->intensity = 0;

		GameObject* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.97f, 0.96f, 0.98f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(0.4, -0.6, 2);
		sun->getComponent<DirectionalLight>()->intensity = 100.0f;
		//sun->setEnable(false);

		GameObject* spot = instanciate("Spot");
		spot->getComponent<Transform>()->setPosition(Vector3f(1.7, 3, -1));
		spot->getComponent<Transform>()->setRotation(Vector3f(-PI/2.0f, 0, 0));
		spot->addComponent<SpotLight>();
		spot->getComponent<SpotLight>()->coneAngle = 0.97;
		spot->getComponent<SpotLight>()->coneAttenuation = 0.96 ;
		spot->getComponent<SpotLight>()->constant = 2;
		spot->getComponent<SpotLight>()->linear = 0.09f;
		spot->getComponent<SpotLight>()->quadratic = 0.032f;
		spot->getComponent<SpotLight>()->color = Vector3f(0.97f, 0.96f, 0.98f);
		spot->getComponent<SpotLight>()->intensity = 1.0f;
		spot->setEnable(false);
		//spot->getComponent<Transform>()->setParent(cam);


		GameObject* rig = instanciate("Rig");
		rig->addComponent<TransformerScript>();

		GameObject* pointLight = instanciate("Pointlight");
		pointLight->addComponent<PointRenderer>();
		pointLight->getComponent<Transform>()->setPosition(Vector3f(1.3, 1.2f, 0));
		pointLight->addComponent<PointLight>();
		pointLight->getComponent<PointLight>()->color = Vector3f(255 / 255.0f, 249 / 255.0f, 225 / 255.0f);
		pointLight->getComponent<PointLight>()->constant = 2;
		pointLight->getComponent<PointLight>()->linear = 0.09f;
		pointLight->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight->getComponent<PointLight>()->intensity = 80.0f;
		pointLight->getComponent<Transform>()->setParent(rig);
		pointLight->setEnable(false);

		GameObject* pointLight2 = instanciate("Pointlight2");
		pointLight2->addComponent<PointRenderer>();
		pointLight2->getComponent<Transform>()->setPosition(Vector3f(0, 3, 9));
		pointLight2->addComponent<PointLight>();
		pointLight2->getComponent<PointLight>()->color = Vector3f(255 / 255.0f, 30 / 255.0f, 50 / 255.0f);
		pointLight2->getComponent<PointLight>()->constant = 2;
		pointLight2->getComponent<PointLight>()->linear = 0.09f;
		pointLight2->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight2->getComponent<PointLight>()->intensity = 4.0f;
		pointLight2->setEnable(false);



		/* --------------------------- OBJECTSS --------------------------- */

		GameObject* couch = instanciate("Couch");
		couch->addComponent<MeshRenderer>();
		couch->getComponent<MeshRenderer>()->setMesh(couchMesh);
		couch->getComponent<MeshRenderer>()->setMaterial(couchMaterial);

		GameObject* longCouch = instanciate("Long-Couch");
		longCouch->addComponent<MeshRenderer>();
		longCouch->getComponent<MeshRenderer>()->setMesh(longCouchMesh);
		longCouch->getComponent<MeshRenderer>()->setMaterial(couchMaterial);

		GameObject* table = instanciate("Rable");
		table->addComponent<MeshRenderer>()->captureble = false;
		table->getComponent<MeshRenderer>()->setMesh(tableMesh);
		table->getComponent<MeshRenderer>()->setMaterial(tableMaterial);
		//table->setEnable(false);

		GameObject* shelve = instanciate("Shelve");
		shelve->addComponent<MeshRenderer>();
		shelve->getComponent<MeshRenderer>()->setMesh(shelveMesh);
		shelve->getComponent<MeshRenderer>()->setMaterial(shelveMaterial);

		GameObject* room = instanciate("Room");
		room->addComponent<MeshRenderer>();
		room->getComponent<MeshRenderer>()->setMesh(wallMesh);
		room->getComponent<MeshRenderer>()->setMaterial(wallMaterial);

		GameObject* occluder = instanciate("Occluder");
		occluder->addComponent<MeshRenderer>();
		occluder->getComponent<MeshRenderer>()->setMesh(occluderMesh);
		occluder->getComponent<MeshRenderer>()->setMaterial(occluderMaterial);

		GameObject* floor = instanciate("Floor");
		floor->addComponent<MeshRenderer>();
		floor->getComponent<MeshRenderer>()->setMesh(floorMesh);
		floor->getComponent<MeshRenderer>()->setMaterial(wood);

		GameObject* floorEdge = instanciate("FloorEdge");
		floorEdge->addComponent<MeshRenderer>();
		floorEdge->getComponent<MeshRenderer>()->setMesh(floorEdgeMesh);
		floorEdge->getComponent<MeshRenderer>()->setMaterial(wood);

		GameObject* doors = instanciate("Doors");
		doors->addComponent<MeshRenderer>();
		doors->getComponent<MeshRenderer>()->setMesh(doorsMesh);
		doors->getComponent<MeshRenderer>()->setMaterial(doorMaterial);

		GameObject* slidingDoor = instanciate("Sliding-Doors");
		slidingDoor->addComponent<MeshRenderer>();
		slidingDoor->getComponent<MeshRenderer>()->setMesh(slidingDoorMesh);
		slidingDoor->getComponent<MeshRenderer>()->setMaterial(slidingDoorMaterial);

		GameObject* carpet = instanciate("Carpet");
		carpet->addComponent<TransparentMeshRenderer>();
		carpet->getComponent<TransparentMeshRenderer>()->setMesh(carpetMesh);
		carpet->getComponent<TransparentMeshRenderer>()->setMaterial(carpetMaterial);

		GameObject* carpetFloor = instanciate("CarpetFloor");
		carpetFloor->addComponent<MeshRenderer>();
		carpetFloor->getComponent<MeshRenderer>()->setMesh(carpetFloorMesh);
		carpetFloor->getComponent<MeshRenderer>()->setMaterial(carpetFloorMaterial);

		GameObject* lamp = instanciate("Lamp");
		lamp->addComponent<MeshRenderer>();
		lamp->getComponent<MeshRenderer>()->setMesh(lampMesh);
		lamp->getComponent<MeshRenderer>()->setMaterial(lampMaterial);

		GameObject* ceilingLight = instanciate("CeilingLight");
		ceilingLight->addComponent<MeshRenderer>();
		ceilingLight->getComponent<MeshRenderer>()->setMesh(ceilingLightMesh);
		ceilingLight->getComponent<MeshRenderer>()->setMaterial(ceilingLightMaterial);

		GameObject* vase = instanciate("Vase");
		vase->addComponent<MeshRenderer>();
		vase->getComponent<MeshRenderer>()->setMesh(vaseMesh);
		vase->getComponent<MeshRenderer>()->setMaterial(vaseMaterial);

		GameObject* book1 = instanciate("Book1");
		book1->addComponent<MeshRenderer>();
		book1->getComponent<MeshRenderer>()->setMesh(bookMesh1);
		book1->getComponent<MeshRenderer>()->setMaterial(bookMaterial1);
		GameObject* book2 = instanciate("Book2");
		book2->addComponent<MeshRenderer>();
		book2->getComponent<MeshRenderer>()->setMesh(bookMesh2);
		book2->getComponent<MeshRenderer>()->setMaterial(bookMaterial2);
		GameObject* book3 = instanciate("Book3");
		book3->addComponent<MeshRenderer>();
		book3->getComponent<MeshRenderer>()->setMesh(bookMesh3);
		book3->getComponent<MeshRenderer>()->setMaterial(bookMaterial3);
		GameObject* book4 = instanciate("Book4");
		book4->addComponent<MeshRenderer>();
		book4->getComponent<MeshRenderer>()->setMesh(bookMesh4);
		book4->getComponent<MeshRenderer>()->setMaterial(bookMaterial4);

		GameObject* balconyFloor = instanciate("Balcony Floor");
		balconyFloor->addComponent<MeshRenderer>();
		balconyFloor->getComponent<MeshRenderer>()->setMesh(balconyFloorMesh);
		balconyFloor->getComponent<MeshRenderer>()->setMaterial(balconyFloorMaterial);

		GameObject* balconyRailing = instanciate("Balcony Railing");
		balconyRailing->addComponent<MeshRenderer>();
		balconyRailing->getComponent<MeshRenderer>()->setMesh(balconyRailingMesh);
		balconyRailing->getComponent<MeshRenderer>()->setMaterial(railingMaterial);

		GameObject* probe = instanciate("Probe");
		probe->getComponent<Transform>()->setPosition(Vector3f(0, 1.2, 0));
		probe->getComponent<Transform>()->setScale(Vector3f(0.1f, 0.1f, 0.1f));
		probe->addComponent<MeshRenderer>();
		probe->getComponent<MeshRenderer>()->setMesh(sphere);
		probe->getComponent<MeshRenderer>()->setMaterial(def);

		GameObject* window = instanciate("Window");
		window->addComponent<TransparentMeshRenderer>()->cutout = false;
		window->getComponent<TransparentMeshRenderer>()->setMesh(windowMesh);
		window->getComponent<TransparentMeshRenderer>()->setMaterial(glassMaterial);
		//window->setEnable(false);

		
		GameObject* panorama = instanciate("Panorama");
		panorama->addComponent<MeshRenderer>();
		panorama->getComponent<MeshRenderer>()->setMesh(panoramaMesh);
		panorama->getComponent<MeshRenderer>()->setMaterial(panoramaMaterial);
		

/*
		for (int x = 0; x < 8; x++) {
			for (int z = 0; z < 10; z++) {
				for (int y = 0; y < 4; y++) {
					GameObject* probe = instanciate("Probe" + x + y + z);
					probe->getComponent<Transform>()->setPosition(Vector3f(x-4, y, z-5));
					probe->getComponent<Transform>()->setScale(Vector3f(0.1f, 0.1f, 0.1f));
					probe->addComponent<MeshRenderer>();
					probe->getComponent<MeshRenderer>()->setMesh(sphere);
					probe->getComponent<MeshRenderer>()->setMaterial(def);
				}
			}
		}*/

	}

	void OnUnload() {
		delete sky;

		delete plane;

		delete default_normal;
		delete default_specular;
		delete default_emission;
		delete default_ao;
		delete gradient;

		delete def;
	}

};
#endif // TEST_SCENE_H