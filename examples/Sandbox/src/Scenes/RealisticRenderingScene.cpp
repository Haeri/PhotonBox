#ifndef REALISTIC_RENDERING_SCENE_CPP
#define REALISTIC_RENDERING_SCENE_CPP

#include <PhotonBox/component/MeshRenderer.h>
#include <PhotonBox/component/Camera.h>
#include <PhotonBox/component/SpotLight.h>
#include <PhotonBox/component/PointLight.h>
#include <PhotonBox/component/PointRenderer.h>
#include <PhotonBox/component/AmbientLight.h>
#include <PhotonBox/component/LightProbe.h>
#include <PhotonBox/resource/Texture.h>
#include <PhotonBox/resource/Scene.h>
#include <PhotonBox/resource/shader/BasicShader.h>
#include <PhotonBox/resource/shader/UnlitShader.h>
#include <PhotonBox/resource/shader/IrradianceShader.h>
#include <PhotonBox/resource/shader/TransparentShader.h>
#include <PhotonBox/resource/shader/SkyBoostShader.h>
#include <PhotonBox/resource/shader/GShader.h>

#include "../PostProcessors/SSAOProcessor.cpp"
#include "../PostProcessors/SSReflectionProcessor.cpp"
#include "../PostProcessors/AutoExposureProcessor.cpp"
#include "../PostProcessors/ToneMappingProcessor.cpp"
#include "../PostProcessors/DOFProcessor.cpp"
#include "../PostProcessors/TAAProcessor.cpp"
#include "../Scripts/MaterialScript.cpp"
#include "../PostProcessors/BloomProcessor.cpp"
#include "../PostProcessors/BlurProcessor.cpp"
#include "../Scripts/PrinterScript.cpp"
#include "../Scripts/TransformerScript.cpp"
#include "../Scripts/CameraControllerScript.cpp"
#include "../Scripts/StateControllerScript.cpp"
#include "../Scripts/PathWalkerScript.cpp"
#include "../Scripts/ControllerToggleScript.cpp"
#include "../Shader/BlurShader.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class RealisticRenderingScene : public Scene
{
public:

	void Load()
	{
		/* --------------------------- ENVIROMENT --------------------------- */
		std::vector<std::string> white = {
			Resources::ENGINE_RESOURCES + "/default_ao.png",
			Resources::ENGINE_RESOURCES + "/default_ao.png",
			Resources::ENGINE_RESOURCES + "/default_ao.png",
			Resources::ENGINE_RESOURCES + "/default_ao.png",
			Resources::ENGINE_RESOURCES + "/default_ao.png",
			Resources::ENGINE_RESOURCES + "/default_ao.png",
		};

		Renderer::setSkyBox(createResource<CubeMap>("WhiteCube", white));
		Renderer::getSkyBox()->intensity = 10;


		/* --------------------------- POST PROCESSING --------------------------- */
		new SSAOProcessor(10);
		new SSReflectionProcessor(20);
		new AutoExposureProcessor(30);
		new TAAProcessor(40);
		new BloomProcessor(60);
		new ToneMappingProcessor(70);
		//new DOFProcessor(100);
		


		/* --------------------------- OBJ --------------------------- */
		Mesh* sphere			= createResource<Mesh>(Filepath(Resources::ENGINE_RESOURCES + "/primitives/sphere.obj"));
		Mesh* couchMesh			= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Couch/Couch.obj"));
		Mesh* wallMesh			= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Walls/Room.obj"));
		Mesh* floorMesh			= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Floor/Floor.obj"));
		Mesh* doorsMesh			= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Doors/Doors.obj"));
		Mesh* tableMesh			= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Table/Table.obj"));
		Mesh* longCouchMesh		= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Couch/Long_Couch.obj"));
		Mesh* slidingDoorMesh	= createResource<Mesh>(Filepath("./res/Realistic-Rendering/SlidingDoor/SlideDoor.obj"));
		Mesh* floorEdgeMesh		= createResource<Mesh>(Filepath("./res/Realistic-Rendering/FloorEdge/FloorEdge.obj"));
		Mesh* shelveMesh		= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Shelving/Shelving.obj"));
		Mesh* carpetMesh		= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Carpet/Carpet.obj"));
		Mesh* carpetFloorMesh	= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Carpet/CarpetFloor.obj"));
		Mesh* occluderMesh		= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Occluder/occluder.obj"));
		Mesh* windowMesh		= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Window/Window.obj"));
		Mesh* panoramaMesh		= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Panorama/Panorama.obj"));
		Mesh* lampMesh			= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Lamp/Lamp.obj"));
		Mesh* ceilingLightMesh	= createResource<Mesh>(Filepath("./res/Realistic-Rendering/CeilingLight/CeilingLight.obj"));
		Mesh* vaseMesh			= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Vase/Vase.obj"));
		Mesh* bookMesh1			= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Books/Books_1.obj"));
		Mesh* bookMesh2			= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Books/Books_2.obj"));
		Mesh* bookMesh3			= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Books/Books_3.obj"));
		Mesh* bookMesh4			= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Books/Books_4.obj"));
		Mesh* balconyFloorMesh	= createResource<Mesh>(Filepath("./res/Realistic-Rendering/BalconyFloor/Balcony_Floor.obj"));
		Mesh* balconyRailingMesh = createResource<Mesh>(Filepath("./res/Realistic-Rendering/Railing/Balcony_Railing.obj"));
		Mesh* frameMesh			= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Frame/Frame.obj"));
		Mesh* frameGlassMesh	= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Frame/GlassPane.obj"));
		Mesh* painting1Mesh		= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Frame/Painting1.obj"));
		Mesh* painting2Mesh		= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Frame/Painting2.obj"));
		Mesh* painting3Mesh		= createResource<Mesh>(Filepath("./res/Realistic-Rendering/Frame/Painting3.obj"));


		/* --------------------------- TEXTURES --------------------------- */
		Texture::Config m_conf;
		m_conf.mips = true;

		Texture* default_normal		= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_normal.png"));
		Texture* default_specular	= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_specular.png"));
		Texture* default_emission	= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_emission.png"));
		Texture* default_ao			= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_ao.png"));
		Texture* default_roughness	= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_roughness.png"));

		Texture* woodAlbedo			= createResource<Texture>(Filepath("./res/materials/mahogfloor/mahogfloor_basecolor.png"), m_conf);
		Texture* woodRough			= createResource<Texture>(Filepath("./res/materials/mahogfloor/mahogfloor_roughness.png"), m_conf);
		Texture* woodNormal			= createResource<Texture>(Filepath("./res/materials/mahogfloor/mahogfloor_normal.png"), m_conf);
		Texture* woodAo				= createResource<Texture>(Filepath("./res/materials/mahogfloor/mahogfloor_AO.png"), m_conf);
		Texture* woodMetal			= createResource<Texture>(Filepath("./res/materials/mahogfloor/mahogfloor_metalness.png"), m_conf);

		Texture* doorAlbedo			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Doors/T_Door_D.tga"), m_conf);
		Texture* doorRough			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Doors/T_Door_R.tga"), m_conf);
		Texture* doorNormal			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Doors/T_Door_N.tga"), m_conf);
		Texture* doorAo				= createResource<Texture>(Filepath("./res/Realistic-Rendering/Doors/T_Door_AOMask.tga"), m_conf);
		Texture* doorMetal			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Doors/T_Door_M.tga"), m_conf);

		Texture* tableAlbedo		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Table/T_Table_D.tga"), m_conf);
		Texture* tableRough			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Table/T_Table_R.tga"), m_conf);
		Texture* tableNormal		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Table/T_Table_N.tga"), m_conf);
		Texture* tableAo			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Table/T_Table_AOMASK.tga"), m_conf);
		Texture* tableMetal			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Table/T_Table_M.tga"), m_conf);

		Texture* slidingDoorRough	= createResource<Texture>(Filepath("./res/Realistic-Rendering/SlidingDoor/T_SlidingDoor_R.tga"), m_conf);
		Texture* slidingDoorNormal	= createResource<Texture>(Filepath("./res/Realistic-Rendering/SlidingDoor/T_SlidingDoor_N.tga"), m_conf);
		Texture* slidingDoorAo		= createResource<Texture>(Filepath("./res/Realistic-Rendering/SlidingDoor/T_SlidingDoor_AOMask.tga"), m_conf);
		Texture* slidingDoorMetal	= createResource<Texture>(Filepath("./res/Realistic-Rendering/SlidingDoor/T_SlidingDoor_M.tga"), m_conf);

		Texture* shelveAlbedo		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Shelving/T_Shelving_D.tga"), m_conf);
		Texture* shelveRough		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Shelving/T_Shelving_R.tga"), m_conf);
		Texture* shelveNormal		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Shelving/T_Shelving_N.tga"), m_conf);
		Texture* shelveAo			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Shelving/T_Shelving_AO.tga"), m_conf);

		Texture* carpetAlbedo		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Carpet/T_Carpet_D.png"), m_conf);
		Texture* carpetNormal		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Carpet/T_Carpet_N.tga"), m_conf);
		Texture* carpetAo			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Carpet/T_Carpet_AO.tga"), m_conf);

		Texture* lampAlbedo			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Lamp/T_FloorLamp_Albedo.tga"), m_conf);
		Texture* lampRough			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Lamp/T_FloorLamp_Roughness.tga"), m_conf);
		Texture* lampNormal			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Lamp/T_FloorLamp_N.tga"), m_conf);
		Texture* lampAo				= createResource<Texture>(Filepath("./res/Realistic-Rendering/Lamp/T_FloorLamp_AO.tga"), m_conf);
		Texture* lampMetal			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Lamp/T_FloorLamp_M.tga"), m_conf);

		Texture* ceilingAlbedo		= createResource<Texture>(Filepath("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_Albedo.tga"), m_conf);
		Texture* ceilingRough		= createResource<Texture>(Filepath("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_Roughness.tga"), m_conf);
		Texture* ceilingNormal		= createResource<Texture>(Filepath("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_N.tga"), m_conf);
		Texture* ceilingAo			= createResource<Texture>(Filepath("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_AO.tga"), m_conf);
		Texture* ceilingMetal		= createResource<Texture>(Filepath("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_M.tga"), m_conf);

		Texture* vaseAlbedo			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Vase/Vase_Albedo.png"), m_conf);
		Texture* vaseRough			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Vase/Vase_Roughness.png"), m_conf);

		Texture* frameAlbedo		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Frame/T_Frame_Albedo.tga"), m_conf);
		Texture* frameNormal		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Frame/T_Frame_N.tga"), m_conf);
		Texture* frameRoughness		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Frame/T_Frame_Roughness.tga"), m_conf);

		Texture* painting1D			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Frame/T_Painting4_D.tga"), m_conf);
		Texture* painting2D			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Frame/T_Painting2_D.tga"), m_conf);
		Texture* painting3D			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Frame/T_Painting3_D.tga"), m_conf);

		Texture* bookAo				= createResource<Texture>(Filepath("./res/Realistic-Rendering/Books/book_occlusion.tga"), m_conf);
		Texture* bookNormal			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Books/book_normals.tga"), m_conf);
		Texture* bookAlbedo1		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Books/book_albedo_1.tga"), m_conf);
		Texture* bookAlbedo2		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Books/book_albedo_2.tga"), m_conf);
		Texture* bookAlbedo3		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Books/book_albedo_3.tga"), m_conf);
		Texture* bookAlbedo4		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Books/book_albedo_4.tga"), m_conf);

		Texture* balconyFloorAlbedo	= createResource<Texture>(Filepath("./res/Realistic-Rendering/BalconyFloor/T_StoneMix_D.tga"), m_conf);
		Texture* balconyFloorNormal	= createResource<Texture>(Filepath("./res/Realistic-Rendering/BalconyFloor/T_StoneMix_N.tga"), m_conf);
		Texture* balconyFloorRoughness = createResource<Texture>(Filepath("./res/Realistic-Rendering/BalconyFloor/T_StoneMix_R.tga"), m_conf);

		Texture* galvanizedAlbedo	= createResource<Texture>(Filepath("./res/Realistic-Rendering/Railing/T_Galvanized_D.tga"), m_conf);
		Texture* galvanizedRough	= createResource<Texture>(Filepath("./res/Realistic-Rendering/Railing/T_Galvanized_Roughness.tga"), m_conf);
		Texture* galvanizedNormal	= createResource<Texture>(Filepath("./res/Realistic-Rendering/Railing/T_Galvanized_N.tga"), m_conf);
		Texture* railingAo			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Railing/T_Railing_AOMASK.tga"), m_conf);
		Texture* railingNormal		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Railing/T_Railing_N.tga"), m_conf);

		Texture* windowAlbedo		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Window/albedo.png"), m_conf);

		Texture* couchAlbedo		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Couch/T_Couch_D.tga"), m_conf);
		Texture* couchNormal		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Couch/T_Couch_N.tga"), m_conf);
		Texture* couchRoughness		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Couch/T_Couch_R.tga"), m_conf);
		Texture* couchAo			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Couch/T_Couch_AO.tga"), m_conf);
		Texture* couchMetal			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Couch/T_Couch_M.tga"), m_conf);

		Texture* wallNormal			= createResource<Texture>(Filepath("./res/Realistic-Rendering/Walls/T_PaintedWall_N.tga"), m_conf);

		Texture* panoramaAlbedo		= createResource<Texture>(Filepath("./res/Realistic-Rendering/Panorama/T_Background_D.tga"), m_conf);
		Texture* blinds				= createResource<Texture>(Filepath("./res/textures/blinds.png"), m_conf);


		/* --------------------------- SHADERS --------------------------- */
		GShader* defaultShader					= GShader::getInstance();
		TransparentShader* transparentShader	= TransparentShader::getInstance();
		UnlitShader* unlitShader				= UnlitShader::getInstance();
		SkyBoostShader* skyBoost				= SkyBoostShader::getInstance();


		/* --------------------------- MATERIALS --------------------------- */

		Material* couchMaterial = createResource<Material>("couchMaterial", defaultShader);
		couchMaterial->setImageBuffer("albedoMap", couchAlbedo);
		couchMaterial->setImageBuffer("normalMap", couchNormal);
		couchMaterial->setImageBuffer("roughnessMap", couchRoughness);
		couchMaterial->setImageBuffer("aoMap", couchAo);
		couchMaterial->setImageBuffer("metallicMap", couchMetal);
		couchMaterial->setImageBuffer("emissionMap", default_emission);

		Material* wallMaterial = createResource<Material>("wallMaterial", defaultShader);
		wallMaterial->setImageBuffer("albedoMap", default_ao);
		wallMaterial->setImageBuffer("normalMap", wallNormal);
		wallMaterial->setImageBuffer("roughnessMap", default_ao);
		wallMaterial->setImageBuffer("aoMap", default_ao);
		wallMaterial->setImageBuffer("metallicMap", default_emission);
		wallMaterial->setImageBuffer("emissionMap", default_emission);

		Material* wood = createResource<Material>("WoodMaterial", defaultShader);
		wood->setImageBuffer("albedoMap", woodAlbedo);
		wood->setImageBuffer("normalMap", woodNormal);
		wood->setImageBuffer("roughnessMap", woodRough);
		wood->setImageBuffer("aoMap", woodAo);
		wood->setImageBuffer("metallicMap", woodMetal);
		wood->setImageBuffer("emissionMap", default_emission);

		Material* doorMaterial = createResource<Material>("doorMaterial", defaultShader);
		doorMaterial->setImageBuffer("albedoMap", doorAlbedo);
		doorMaterial->setImageBuffer("normalMap", doorNormal);
		doorMaterial->setImageBuffer("roughnessMap", doorRough);
		doorMaterial->setImageBuffer("aoMap", doorAo);
		doorMaterial->setImageBuffer("metallicMap", doorMetal);
		doorMaterial->setImageBuffer("emissionMap", default_emission);

		Material* tableMaterial = createResource<Material>("tableMaterial", defaultShader);
		tableMaterial->setImageBuffer("albedoMap", tableAlbedo);
		tableMaterial->setImageBuffer("normalMap", tableNormal);
		tableMaterial->setImageBuffer("roughnessMap", tableRough);
		tableMaterial->setImageBuffer("aoMap", tableAo);
		tableMaterial->setImageBuffer("metallicMap", tableMetal);
		tableMaterial->setImageBuffer("emissionMap", default_emission);

		Material* shelveMaterial = createResource<Material>("shelveMaterial", defaultShader);
		shelveMaterial->setImageBuffer("albedoMap", shelveAlbedo);
		shelveMaterial->setImageBuffer("normalMap", shelveNormal);
		shelveMaterial->setImageBuffer("roughnessMap", shelveRough);
		shelveMaterial->setImageBuffer("aoMap", shelveAo);
		shelveMaterial->setImageBuffer("metallicMap", default_emission);
		shelveMaterial->setImageBuffer("emissionMap", default_emission);

		Material* slidingDoorMaterial = createResource<Material>("slidingDoorMaterial", defaultShader);
		slidingDoorMaterial->setImageBuffer("albedoMap", default_ao);
		slidingDoorMaterial->setImageBuffer("normalMap", slidingDoorNormal);
		slidingDoorMaterial->setImageBuffer("roughnessMap", slidingDoorRough);
		slidingDoorMaterial->setImageBuffer("aoMap", slidingDoorAo);
		slidingDoorMaterial->setImageBuffer("metallicMap", slidingDoorMetal);
		slidingDoorMaterial->setImageBuffer("emissionMap", default_emission);

		Material* carpetMaterial = createResource<Material>("carpetMaterial", defaultShader);
		carpetMaterial->setImageBuffer("albedoMap", carpetAlbedo);
		carpetMaterial->setImageBuffer("normalMap", default_normal);
		carpetMaterial->setImageBuffer("roughnessMap", default_ao);
		carpetMaterial->setImageBuffer("aoMap", carpetAo);
		carpetMaterial->setImageBuffer("metallicMap", default_emission);
		carpetMaterial->setImageBuffer("emissionMap", default_emission);

		Material* carpetFloorMaterial = createResource<Material>("carpetFloorMaterial", defaultShader);
		carpetFloorMaterial->setImageBuffer("albedoMap", carpetAlbedo);
		carpetFloorMaterial->setImageBuffer("normalMap", default_normal);
		carpetFloorMaterial->setImageBuffer("roughnessMap", default_ao);
		carpetFloorMaterial->setImageBuffer("aoMap", carpetAo);
		carpetFloorMaterial->setImageBuffer("metallicMap", default_emission);
		carpetFloorMaterial->setImageBuffer("emissionMap", default_emission);

		Material* lampMaterial = createResource<Material>("lampMaterial", defaultShader);
		lampMaterial->setImageBuffer("albedoMap", lampAlbedo);
		lampMaterial->setImageBuffer("normalMap", lampNormal);
		lampMaterial->setImageBuffer("roughnessMap", lampRough);
		lampMaterial->setImageBuffer("aoMap", lampAo);
		lampMaterial->setImageBuffer("metallicMap", lampMetal);
		lampMaterial->setImageBuffer("emissionMap", default_emission);

		Material* ceilingLightMaterial = createResource<Material>("ceilingLightMaterial", defaultShader);
		ceilingLightMaterial->setImageBuffer("albedoMap", ceilingAlbedo);
		ceilingLightMaterial->setImageBuffer("normalMap", ceilingNormal);
		ceilingLightMaterial->setImageBuffer("roughnessMap", ceilingRough);
		ceilingLightMaterial->setImageBuffer("aoMap", ceilingAo);
		ceilingLightMaterial->setImageBuffer("metallicMap", ceilingMetal);
		ceilingLightMaterial->setImageBuffer("emissionMap", default_emission);

		Material* vaseMaterial = createResource<Material>("vaseMaterial", defaultShader);
		vaseMaterial->setImageBuffer("albedoMap", vaseAlbedo);
		vaseMaterial->setImageBuffer("normalMap", default_normal);
		vaseMaterial->setImageBuffer("roughnessMap", vaseRough);
		vaseMaterial->setImageBuffer("aoMap", default_ao);
		vaseMaterial->setImageBuffer("metallicMap", default_emission);
		vaseMaterial->setImageBuffer("emissionMap", default_emission);

		Material* frameMaterial = createResource<Material>("frameMaterial", defaultShader);
		frameMaterial->setImageBuffer("albedoMap", frameAlbedo);
		frameMaterial->setImageBuffer("normalMap", frameNormal);
		frameMaterial->setImageBuffer("roughnessMap", frameRoughness);
		frameMaterial->setImageBuffer("aoMap", default_ao);
		frameMaterial->setImageBuffer("metallicMap", default_emission);
		frameMaterial->setImageBuffer("emissionMap", default_emission);

		Material* painting1Material = createResource<Material>("painting1Material", defaultShader);
		painting1Material->setImageBuffer("albedoMap", painting1D);
		painting1Material->setImageBuffer("normalMap", default_normal);
		painting1Material->setImageBuffer("roughnessMap", default_ao);
		painting1Material->setImageBuffer("aoMap", default_ao);
		painting1Material->setImageBuffer("metallicMap", default_emission);
		painting1Material->setImageBuffer("emissionMap", default_emission);
		Material* painting2Material = createResource<Material>("painting2Material", defaultShader);
		painting2Material->setImageBuffer("albedoMap", painting2D);
		painting2Material->setImageBuffer("normalMap", default_normal);
		painting2Material->setImageBuffer("roughnessMap", default_ao);
		painting2Material->setImageBuffer("aoMap", default_ao);
		painting2Material->setImageBuffer("metallicMap", default_emission);
		painting2Material->setImageBuffer("emissionMap", default_emission);
		Material* painting3Material = createResource<Material>("painting3Material", defaultShader);
		painting3Material->setImageBuffer("albedoMap", painting3D);
		painting3Material->setImageBuffer("normalMap", default_normal);
		painting3Material->setImageBuffer("roughnessMap", default_ao);
		painting3Material->setImageBuffer("aoMap", default_ao);
		painting3Material->setImageBuffer("metallicMap", default_emission);
		painting3Material->setImageBuffer("emissionMap", default_emission);


		Material* bookMaterial1 = createResource<Material>("bookMaterial1", defaultShader);
		bookMaterial1->setImageBuffer("albedoMap", bookAlbedo1);
		bookMaterial1->setImageBuffer("normalMap", bookNormal);
		bookMaterial1->setImageBuffer("roughnessMap", default_ao);
		bookMaterial1->setImageBuffer("aoMap", bookAo);
		bookMaterial1->setImageBuffer("metallicMap", default_emission);
		bookMaterial1->setImageBuffer("emissionMap", default_emission);
		Material* bookMaterial2 = createResource<Material>("bookMaterial2", defaultShader);
		bookMaterial2->setImageBuffer("albedoMap", bookAlbedo2);
		bookMaterial2->setImageBuffer("normalMap", bookNormal);
		bookMaterial2->setImageBuffer("roughnessMap", default_ao);
		bookMaterial2->setImageBuffer("aoMap", bookAo);
		bookMaterial2->setImageBuffer("metallicMap", default_emission);
		bookMaterial2->setImageBuffer("emissionMap", default_emission);
		Material* bookMaterial3 = createResource<Material>("bookMaterial3", defaultShader);
		bookMaterial3->setImageBuffer("albedoMap", bookAlbedo3);
		bookMaterial3->setImageBuffer("normalMap", bookNormal);
		bookMaterial3->setImageBuffer("roughnessMap", default_ao);
		bookMaterial3->setImageBuffer("aoMap", bookAo);
		bookMaterial3->setImageBuffer("metallicMap", default_emission);
		bookMaterial3->setImageBuffer("emissionMap", default_emission);
		Material* bookMaterial4 = createResource<Material>("bookMaterial4", defaultShader);
		bookMaterial4->setImageBuffer("albedoMap", bookAlbedo4);
		bookMaterial4->setImageBuffer("normalMap", bookNormal);
		bookMaterial4->setImageBuffer("roughnessMap", default_ao);
		bookMaterial4->setImageBuffer("aoMap", bookAo);
		bookMaterial4->setImageBuffer("metallicMap", default_emission);
		bookMaterial4->setImageBuffer("emissionMap", default_emission);

		Material* railingMaterial = createResource<Material>("railingMaterial", defaultShader);
		railingMaterial->setImageBuffer("albedoMap", galvanizedAlbedo);
		railingMaterial->setImageBuffer("normalMap", railingNormal);
		railingMaterial->setImageBuffer("roughnessMap", galvanizedRough);
		railingMaterial->setImageBuffer("aoMap", railingAo);
		railingMaterial->setImageBuffer("metallicMap", default_ao);
		railingMaterial->setImageBuffer("emissionMap", default_emission);

		Material* balconyFloorMaterial = createResource<Material>("balconyFloorMaterial", defaultShader);
		balconyFloorMaterial->setImageBuffer("albedoMap", balconyFloorAlbedo);
		balconyFloorMaterial->setImageBuffer("normalMap", balconyFloorNormal);
		balconyFloorMaterial->setImageBuffer("roughnessMap", balconyFloorRoughness);
		balconyFloorMaterial->setImageBuffer("aoMap", default_ao);
		balconyFloorMaterial->setImageBuffer("metallicMap", default_emission);
		balconyFloorMaterial->setImageBuffer("emissionMap", default_emission);

		Material* blindsMaterial = createResource<Material>("blindsMaterial", defaultShader);
		blindsMaterial->setImageBuffer("albedoMap", blinds);
		blindsMaterial->setImageBuffer("normalMap", default_normal);
		blindsMaterial->setImageBuffer("roughnessMap", default_emission);
		blindsMaterial->setImageBuffer("aoMap", default_ao);
		blindsMaterial->setImageBuffer("metallicMap", default_ao);
		blindsMaterial->setImageBuffer("emissionMap", default_emission);

		Material* def = createResource<Material>("def", defaultShader);
		def->setImageBuffer("albedoMap", default_specular);
		def->setImageBuffer("normalMap", default_normal);
		def->setImageBuffer("roughnessMap", default_emission);
		def->setImageBuffer("aoMap", default_ao);
		def->setImageBuffer("metallicMap", default_ao);
		def->setImageBuffer("emissionMap", default_emission);

		Material* glassMaterial = createResource<Material>("glassMaterial", transparentShader);
		glassMaterial->setImageBuffer("albedoMap", windowAlbedo);
		glassMaterial->setImageBuffer("normalMap", default_normal);
		glassMaterial->setImageBuffer("roughnessMap", default_roughness);
		glassMaterial->setImageBuffer("aoMap", default_ao);
		glassMaterial->setImageBuffer("metallicMap", default_emission);
		glassMaterial->setImageBuffer("emissionMap", default_emission);

		Material* occluderMaterial = createResource<Material>("occluderMaterial", unlitShader);

		Material* panoramaMaterial = createResource<Material>("panoramaMaterial", skyBoost);
		panoramaMaterial->setImageBuffer("albedoMap", panoramaAlbedo);
		//panoramaMaterial->setProperty("boost", 60.0f);

		/* --------------------------- CAMERA --------------------------- */

		std::vector<Vector3f> positions;
		std::vector<Quaternion> rotations;

		positions.push_back(Vector3f(1.027929f, 0.694633f, -2.158259f));
		rotations.push_back(Quaternion(0.000000f, 0.000000f, 0.000000f, 1.000000f));

		positions.push_back(Vector3f(-1.360401f, 0.694633f, -2.075065f));
		rotations.push_back(Quaternion(-0.028378f, -0.230313f, 0.006719f, 0.972680f));

		positions.push_back(Vector3f(-2.056820f, 0.512246f, -0.024637f));
		rotations.push_back(Quaternion(0.009905f, -0.705615f, -0.006306f, 0.703909f));

		positions.push_back(Vector3f(-1.386565f, 0.527661f, -0.021926f));
		rotations.push_back(Quaternion(0.009905f, -0.705615f, -0.006306f, 0.703909f));

		positions.push_back(Vector3f(-1.546286f, 1.844576f, 3.710133f));
		rotations.push_back(Quaternion(-0.003881f, 0.921661f, -0.005412f, 0.374513f));

		positions.push_back(Vector3f(-1.551588f, 0.697409f, 3.701233f));
		rotations.push_back(Quaternion(0.083715f, 0.875332f, 0.169155f, 0.433270f));

		positions.push_back(Vector3f(1.669045f, 1.679932f, -0.204936f));
		rotations.push_back(Quaternion(-0.001640f, 0.990844f, -0.011507f, -0.018861f));

		positions.push_back(Vector3f(1.831609f, 1.694534f, 0.390910f));
		rotations.push_back(Quaternion(-0.001640f, 0.990844f, -0.011507f, -0.018861f));

		positions.push_back(Vector3f(1.302423f, 1.878779f, 3.948425f));
		rotations.push_back(Quaternion(0.024274f, -0.972819f, 0.122343f, -0.131675f));

		positions.push_back(Vector3f(0.219692f, 1.831888f, 3.995026f));
		rotations.push_back(Quaternion(-0.001262f, -0.979887f, 0.116091f, 0.074467f));

		positions.push_back(Vector3f(1.450421f, 2.951797f, -1.219468f));
		rotations.push_back(Quaternion(0.450830f, -0.545559f, 0.454648f, -0.519226f));

		positions.push_back(Vector3f(1.427990f, 2.900677f, 1.304277f));
		rotations.push_back(Quaternion(0.450830f, -0.545559f, 0.454648f, -0.519226f));

		positions.push_back(Vector3f(0.001378f, 0.554877f, 1.378379f));
		rotations.push_back(Quaternion(0.000040f, -0.999493f, -0.020330f, -0.000862f));

		positions.push_back(Vector3f(-0.001278f, 0.617471f, -0.158748f));
		rotations.push_back(Quaternion(0.000040f, -0.999493f, -0.020330f, -0.000862f));



		Entity* cam = instantiate("Camera");
		cam->addComponent<Camera>()->setFOV(60);
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 2, -2));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<StateControllerScript>();
		cam->addComponent<CameraControllerScript>();

		cam->addComponent<PathWalkerScript>()->speed = 0.1f;
		cam->getComponent<PathWalkerScript>()->positions = positions;
		cam->getComponent<PathWalkerScript>()->rotations = rotations;
		cam->addComponent<ControllerToggleScript>();



		/* --------------------------- LIGHT HELPER --------------------------- */

		Entity* lightProbe = instantiate("LightProbe");
		lightProbe->getComponent<Transform>()->setPosition(Vector3f(0, 1.2f, 0));
		lightProbe->addComponent<LightProbe>()->resolution = 512;
		lightProbe->getComponent<LightProbe>()->bounds.setMinBound(Vector3f(-2.1f, -1.3f, -3.4f));
		lightProbe->getComponent<LightProbe>()->bounds.setMaxBound(Vector3f(2.1f, 2.0f, 6));


		Entity* min = instantiate("min");
		min->getComponent<Transform>()->setPosition(Vector3f(-2.1f, -1.3f, -3.4f));

		Entity* max = instantiate("max");
		max->getComponent<Transform>()->setPosition(Vector3f(2.1f, 2.0f, 6));


		/* --------------------------- LIGHTS --------------------------- */
		Entity* ambient = instantiate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(0.3f, 0.3f, 0.3f);
		ambient->getComponent<AmbientLight>()->intensity = 0;

		Entity* sun = instantiate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(255.0f / 255.0f, 248.0f / 255.0f, 240.0f / 255.0f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(0.4f, -0.6f, 2.0f);
		sun->getComponent<DirectionalLight>()->intensity = 800.0f;



		/* --------------------------- OBJECTSS --------------------------- */

		Entity* couch = instantiate("Couch");
		couch->addComponent<MeshRenderer>();
		couch->getComponent<MeshRenderer>()->setMesh(couchMesh);
		couch->getComponent<MeshRenderer>()->setMaterial(couchMaterial);

		Entity* longCouch = instantiate("Long-Couch");
		longCouch->addComponent<MeshRenderer>();
		longCouch->getComponent<MeshRenderer>()->setMesh(longCouchMesh);
		longCouch->getComponent<MeshRenderer>()->setMaterial(couchMaterial);

		Entity* table = instantiate("Rable");
		table->addComponent<MeshRenderer>()->getReflected = false;
		table->getComponent<MeshRenderer>()->setMesh(tableMesh);
		table->getComponent<MeshRenderer>()->setMaterial(tableMaterial);

		Entity* shelve = instantiate("Shelve");
		shelve->addComponent<MeshRenderer>();
		shelve->getComponent<MeshRenderer>()->setMesh(shelveMesh);
		shelve->getComponent<MeshRenderer>()->setMaterial(shelveMaterial);

		Entity* room = instantiate("Room");
		room->addComponent<MeshRenderer>();
		room->getComponent<MeshRenderer>()->setMesh(wallMesh);
		room->getComponent<MeshRenderer>()->setMaterial(wallMaterial);

		Entity* occluder = instantiate("Occluder");
		occluder->addComponent<MeshRenderer>();
		occluder->getComponent<MeshRenderer>()->setMesh(occluderMesh);
		occluder->getComponent<MeshRenderer>()->setMaterial(occluderMaterial);

		Entity* floor = instantiate("Floor");
		floor->addComponent<MeshRenderer>();
		floor->getComponent<MeshRenderer>()->setMesh(floorMesh);
		floor->getComponent<MeshRenderer>()->setMaterial(wood);

		Entity* floorEdge = instantiate("FloorEdge");
		floorEdge->addComponent<MeshRenderer>();
		floorEdge->getComponent<MeshRenderer>()->setMesh(floorEdgeMesh);
		floorEdge->getComponent<MeshRenderer>()->setMaterial(wood);

		Entity* doors = instantiate("Doors");
		doors->addComponent<MeshRenderer>();
		doors->getComponent<MeshRenderer>()->setMesh(doorsMesh);
		doors->getComponent<MeshRenderer>()->setMaterial(doorMaterial);

		Entity* slidingDoor = instantiate("Sliding-Doors");
		slidingDoor->addComponent<MeshRenderer>();
		slidingDoor->getComponent<MeshRenderer>()->setMesh(slidingDoorMesh);
		slidingDoor->getComponent<MeshRenderer>()->setMaterial(slidingDoorMaterial);

		Entity* carpet = instantiate("Carpet");
		carpet->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_CUTOUT);
		carpet->getComponent<MeshRenderer>()->setMesh(carpetMesh);
		carpet->getComponent<MeshRenderer>()->setMaterial(carpetMaterial);

		Entity* carpetFloor = instantiate("CarpetFloor");
		carpetFloor->addComponent<MeshRenderer>();
		carpetFloor->getComponent<MeshRenderer>()->setMesh(carpetFloorMesh);
		carpetFloor->getComponent<MeshRenderer>()->setMaterial(carpetFloorMaterial);

		Entity* lamp = instantiate("Lamp");
		lamp->addComponent<MeshRenderer>();
		lamp->getComponent<MeshRenderer>()->setMesh(lampMesh);
		lamp->getComponent<MeshRenderer>()->setMaterial(lampMaterial);

		Entity* ceilingLight = instantiate("CeilingLight");
		ceilingLight->addComponent<MeshRenderer>();
		ceilingLight->getComponent<MeshRenderer>()->setMesh(ceilingLightMesh);
		ceilingLight->getComponent<MeshRenderer>()->setMaterial(ceilingLightMaterial);

		Entity* vase = instantiate("Vase");
		vase->addComponent<MeshRenderer>();
		vase->getComponent<MeshRenderer>()->setMesh(vaseMesh);
		vase->getComponent<MeshRenderer>()->setMaterial(vaseMaterial);

		Entity* frame = instantiate("Frame");
		frame->addComponent<MeshRenderer>();
		frame->getComponent<MeshRenderer>()->setMesh(frameMesh);
		frame->getComponent<MeshRenderer>()->setMaterial(frameMaterial);

		Entity* frameGlass = instantiate("FrameGlass");
		frameGlass->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_TRANSPARENT);
		frameGlass->getComponent<MeshRenderer>()->setMesh(frameGlassMesh);
		frameGlass->getComponent<MeshRenderer>()->setMaterial(glassMaterial);

		Entity* painting1 = instantiate("Painting1");
		painting1->addComponent<MeshRenderer>();
		painting1->getComponent<MeshRenderer>()->setMesh(painting1Mesh);
		painting1->getComponent<MeshRenderer>()->setMaterial(painting1Material);

		Entity* painting2 = instantiate("Painting2");
		painting2->addComponent<MeshRenderer>();
		painting2->getComponent<MeshRenderer>()->setMesh(painting2Mesh);
		painting2->getComponent<MeshRenderer>()->setMaterial(painting2Material);

		Entity* painting3 = instantiate("Painting3");
		painting3->addComponent<MeshRenderer>();
		painting3->getComponent<MeshRenderer>()->setMesh(painting3Mesh);
		painting3->getComponent<MeshRenderer>()->setMaterial(painting3Material);


		Entity* book1 = instantiate("Book1");
		book1->addComponent<MeshRenderer>();
		book1->getComponent<MeshRenderer>()->setMesh(bookMesh1);
		book1->getComponent<MeshRenderer>()->setMaterial(bookMaterial1);
		Entity* book2 = instantiate("Book2");
		book2->addComponent<MeshRenderer>();
		book2->getComponent<MeshRenderer>()->setMesh(bookMesh2);
		book2->getComponent<MeshRenderer>()->setMaterial(bookMaterial2);
		Entity* book3 = instantiate("Book3");
		book3->addComponent<MeshRenderer>();
		book3->getComponent<MeshRenderer>()->setMesh(bookMesh3);
		book3->getComponent<MeshRenderer>()->setMaterial(bookMaterial3);
		Entity* book4 = instantiate("Book4");
		book4->addComponent<MeshRenderer>();
		book4->getComponent<MeshRenderer>()->setMesh(bookMesh4);
		book4->getComponent<MeshRenderer>()->setMaterial(bookMaterial4);

		Entity* balconyFloor = instantiate("Balcony Floor");
		balconyFloor->addComponent<MeshRenderer>();
		balconyFloor->getComponent<MeshRenderer>()->setMesh(balconyFloorMesh);
		balconyFloor->getComponent<MeshRenderer>()->setMaterial(balconyFloorMaterial);

		Entity* balconyRailing = instantiate("Balcony Railing");
		balconyRailing->addComponent<MeshRenderer>();
		balconyRailing->getComponent<MeshRenderer>()->setMesh(balconyRailingMesh);
		balconyRailing->getComponent<MeshRenderer>()->setMaterial(railingMaterial);

		Entity* probe = instantiate("Probe");
		probe->getComponent<Transform>()->setPosition(Vector3f(0.0f, 1.2f, 0.0f));
		probe->getComponent<Transform>()->setScale(Vector3f(0.1f, 0.1f, 0.1f));
		probe->addComponent<MeshRenderer>();
		probe->getComponent<MeshRenderer>()->setMesh(sphere);
		probe->getComponent<MeshRenderer>()->setMaterial(def);
		probe->getComponent<MeshRenderer>()->getReflected = false;

		Entity* window = instantiate("Window");
		window->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_TRANSPARENT);
		window->getComponent<MeshRenderer>()->setMesh(windowMesh);
		window->getComponent<MeshRenderer>()->setMaterial(glassMaterial);

		Entity* panorama = instantiate("Panorama");
		panorama->addComponent<MeshRenderer>();
		panorama->getComponent<MeshRenderer>()->setMesh(panoramaMesh);
		panorama->getComponent<MeshRenderer>()->setMaterial(panoramaMaterial);
	}

};
#endif // REALISTIC_RENDERING_SCENE_CPP