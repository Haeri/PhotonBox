#include "Core.h"
#include "Space.cpp"
#include "TheRenderer.h"
#include "TheTime.h"
#include "Logic.h"
#include "Physics.h"

Space space;
TheRenderer renderer;
TheTime theTime;
Logic logic;
Physics physics;

double accumulatedTime = 0;
double const FIXED_TIME_INTERVAL = 0.014;

bool isDebug = false;
bool doPostProcessing = true;

GLint lastSpace, lastO, lastP;

/* Cache things */

// Post Processing
ModelPtr cutOffSprite, blurSprite, additivSprite;



/* Initialize the Project */
void Core::init()
{
	bRenderer::loadConfigFile("config.json");	// load custom configurations replacing the default values in Configuration.cpp

												// let the renderer create an OpenGL context and the main window
	if (Input::isTouchDevice())
		bRenderer().initRenderer(true);										// full screen on iOS
	else
		bRenderer().initRenderer(1600, 900, false, "ASTEROIDS 3D");		// windowed mode on desktop
																			//bRenderer().initRenderer(View::getScreenWidth(), View::getScreenHeight(), true);		// full screen using full width and height of the screen

	// start main loop 
	bRenderer().runRenderer();
}

/* This function is executed when initializing the renderer */
void Core::initFunction()
{
	// get OpenGL and shading language version
	bRenderer::log("OpenGL Version: ", glGetString(GL_VERSION));
	bRenderer::log("Shading Language Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// set shader versions (optional)
	bRenderer().getObjects()->setShaderVersionDesktop("#version 120");
	bRenderer().getObjects()->setShaderVersionES("#version 100");
	_running = false;
	_viewMatrixHUD = Camera::lookAt(vmml::Vector3f(0.0f, 0.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);

	space.load();
	renderer.setRenderer(&bRenderer(), &_running);
	renderer.init();
	logic.start();

	// postprocessing
	bRenderer().getObjects()->createFramebuffer("fbo");					// create framebuffer object
	bRenderer().getObjects()->createTexture("fbo_original", 0.f, 0.f);
	bRenderer().getObjects()->createTexture("fbo_texture1", 0.f, 0.f);	// create texture to bind to the fbo
	bRenderer().getObjects()->createTexture("fbo_texture2", 0.f, 0.f);	// create texture to bind to the fbo
	bRenderer().getObjects()->createTexture("fbo_blured", 0.f, 0.f);  
	
	bRenderer().getObjects()->createLight("firstLight", vmml::Vector3f(78.0f, -3.0f, 0.0f), vmml::Vector3f(0.5f, 0.5f, 1.0f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 10000.0f, 0.4f, 9000.0f);
	bRenderer().getObjects()->setAmbientColor(vmml::Vector3f(67/255.0f, 30/255.0f, 90/255.0f));


	ShaderPtr cutOffShader = bRenderer().getObjects()->loadShaderFile("cutOffShader", 0, false, false, false, false, false);			// load shader that blurs the texture
	MaterialPtr cutOffMaterial = bRenderer().getObjects()->createMaterial("cutOffMaterial", cutOffShader);								// create an empty material to assign either texture1 or texture2 to
	cutOffSprite = bRenderer().getObjects()->createSprite("cutOffSprite", cutOffMaterial);																// create a sprite using the material created above

	ShaderPtr blurShader = bRenderer().getObjects()->loadShaderFile("blurShader", 0, false, false, false, false, false);			// load shader that blurs the texture
	MaterialPtr blurMaterial = bRenderer().getObjects()->createMaterial("blurMaterial", blurShader);								// create an empty material to assign either texture1 or texture2 to
	blurSprite = bRenderer().getObjects()->createSprite("blurSprite", blurMaterial);																// create a sprite using the material created above

	ShaderPtr additivShader = bRenderer().getObjects()->loadShaderFile("additivShader", 0, false, false, false, false, false);			// load shader that blurs the texture
	MaterialPtr additivMaterial = bRenderer().getObjects()->createMaterial("additivMaterial", additivShader);								// create an empty material to assign either texture1 or texture2 to
	additivSprite = bRenderer().getObjects()->createSprite("additivSprite", additivMaterial);

	// create text sprites
	FontPtr font = bRenderer().getObjects()->loadFont("arial.ttf", 50);
	bRenderer().getObjects()->createTextSprite("instructions", vmml::Vector3f(1.f, 1.f, 1.f), "Press space to start", font);
    
    // Create camera before loading the space since it its position its needed for the skybox
    bRenderer().getObjects()->createCamera("camera", vmml::Vector3f(0.0f, 0.0f, 10.0f), vmml::Vector3f(0.f, 0.0f, 0.f));

	}

/* Draw your scene here */
void Core::loopFunction(const double &deltaTime, const double &elapsedTime)
{
	theTime.setTime(deltaTime, elapsedTime);

	


	logic.update();
	

	accumulatedTime += deltaTime;
	if (accumulatedTime > FIXED_TIME_INTERVAL) {
		physics.update();
		logic.fixedUpdate();
		accumulatedTime = 0;
	}




	/// Post Processing ///

	// Create a small and draw the scene to it
	GLint defaultFBO;
	vmml::Matrix4f modelMatrix = vmml::create_translation(vmml::Vector3f(0.0f, 0.0f, -0.5));
	if (doPostProcessing) {
		defaultFBO = Framebuffer::getCurrentFramebuffer();	// get current fbo to bind it again after drawing the scene
		bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture("fbo_original"), false);	// bind the fbo
	}

	// Draw scene
	renderer.render();

	bRenderer().getModelRenderer()->drawQueue(/*GL_LINES*/);
	bRenderer().getModelRenderer()->clearQueue();

	
	if (doPostProcessing) {
		/*** CUTOF PROCESS ***/
		// Bind Buffer
		bRenderer().getView()->setViewportSize(bRenderer().getView()->getWidth() / 5, bRenderer().getView()->getHeight() / 5);
		bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture("fbo_texture1"), false);
		// Set shader property
		bRenderer().getObjects()->getMaterial("cutOffMaterial")->setTexture("fbo_texture", bRenderer().getObjects()->getTexture("fbo_original"));
		bRenderer().getObjects()->getMaterial("cutOffMaterial")->setScalar("thresHold", static_cast<GLfloat>(0.5f));
		// Draw
		bRenderer().getModelRenderer()->drawModel(cutOffSprite, modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);




		/*** BLUR PROCESS ***/
		bool b = true;
		int numberOfBlurSteps = 2;

		for (int i = 0; i < numberOfBlurSteps; i++) {
			if (i != numberOfBlurSteps - 1) {
				// Bind Buffer
				bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture(b ? "fbo_texture2" : "fbo_texture1"), false);
			}
			else {
				// Bind Buffer
				bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture("fbo_blured"), false);
			}
			// Set shader property
			bRenderer().getObjects()->getMaterial("blurMaterial")->setTexture("fbo_texture", bRenderer().getObjects()->getTexture(b ? "fbo_texture1" : "fbo_texture2"));
			bRenderer().getObjects()->getMaterial("blurMaterial")->setScalar("isVertical", static_cast<GLfloat>(b));

			// Draw
			bRenderer().getModelRenderer()->drawModel(blurSprite, modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);
			b = !b;
		}



		/*** ADDITIV PROCESS ***/
		// Bind Buffer
		bRenderer().getObjects()->getFramebuffer("fbo")->unbind(defaultFBO); //unbind (original fbo will be bound)
		bRenderer().getView()->setViewportSize(bRenderer().getView()->getWidth(), bRenderer().getView()->getHeight());
		// Set shader property
		bRenderer().getObjects()->getMaterial("additivMaterial")->setTexture("fbo_original", bRenderer().getObjects()->getTexture("fbo_original"));
		bRenderer().getObjects()->getMaterial("additivMaterial")->setTexture("fbo_blurTexture", bRenderer().getObjects()->getTexture("fbo_blured"));
		bRenderer().getObjects()->getMaterial("additivMaterial")->setScalar("intensity", static_cast<GLfloat>(4.0f));
		// Draw
		bRenderer().getModelRenderer()->drawModel(additivSprite, modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);

		/// End Processing ///
	}
	

	/// Update Systems ///
	logic.lateUpdate();




	// Draw UI
	float titleScale = 0.03f;
	vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
	vmml::Matrix4f uiModelMatrix = vmml::create_translation(vmml::Vector3f(-0.98f, 0.95f, -0.65f)) * scaling;
	if (isDebug)
		bRenderer().getObjects()->getTextSprite("instructions")->setText(
			"FPS: " + std::to_string(1 / deltaTime) + "\n" +
			"DeltaTime: " + std::to_string(deltaTime) + "\n" +
			"----------------\n" +
			"GameObjects:\n" + space.getGameObjects() +
			"----------------\n" +
			"Behaviours:\n" + logic.getList() +
			"----------------\n" +
			"Physics:\n" + physics.getList()
		);
	else
		bRenderer().getObjects()->getTextSprite("instructions")->setText("FPS: " + std::to_string(1 / deltaTime));
	bRenderer().getModelRenderer()->drawModel(bRenderer().getObjects()->getTextSprite("instructions"), uiModelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);



	GLint currentP = TheRenderer::Instance()->renderer->getInput()->getKeyState(bRenderer::KEY_P);
	GLint currentO = TheRenderer::Instance()->renderer->getInput()->getKeyState(bRenderer::KEY_O);

	if (currentP != lastP){
		lastP = currentP;
		if (currentP == bRenderer::INPUT_PRESS) {
			doPostProcessing = !doPostProcessing;
		}
	}

	if (currentO != lastO) {
		lastO = currentO;
		if (currentO == bRenderer::INPUT_PRESS) {
			isDebug = !isDebug;
		}
	}

	// Quit renderer when escape is pressed
	if (bRenderer().getInput()->getKeyState(bRenderer::KEY_ESCAPE) == bRenderer::INPUT_PRESS)
		bRenderer().terminateRenderer();
}

/* This function is executed when terminating the renderer */
void Core::terminateFunction()
{
	logic.destroy();
	renderer.destroy();
	space.unload();
}


/* For iOS only: Handle device rotation */
void Core::deviceRotated()
{
	if (bRenderer().isInitialized()) {
		// set view to full screen after device rotation
		bRenderer().getView()->setFullscreen(true);
		bRenderer::log("Device rotated");
	}
}

/* For iOS only: Handle app going into background */
void Core::appWillResignActive()
{
	if (bRenderer().isInitialized()) {
		// stop the renderer when the app isn't active
		bRenderer().stopRenderer();
	}
}

/* For iOS only: Handle app coming back from background */
void Core::appDidBecomeActive()
{
	if (bRenderer().isInitialized()) {
		// run the renderer as soon as the app is active
		bRenderer().runRenderer();
	}
}

/* For iOS only: Handle app being terminated */
void Core::appWillTerminate()
{
	if (bRenderer().isInitialized()) {
		// terminate renderer before the app is closed
		bRenderer().terminateRenderer();
	}
}

/* Helper functions */
GLfloat Core::randomNumber(GLfloat min, GLfloat max) {
	return min + static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / (max - min)));
}