#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

#include "bRenderer.h"

class Core : public IRenderProject
{
public:
	/* Constructor and Destructor */
	Core() : IRenderProject(){}
    virtual ~Core(){bRenderer::log("RenderProject deleted");}

    /* Initialize the Project */
    void init();
    
	//// RenderProject functions ////

	/* This function is executed when initializing the renderer */
	void initFunction();

	/* Draw your scene here */
	void loopFunction(const double &deltaTime, const double &elapsedTime);

	/* This function is executed when terminating the renderer */
	void terminateFunction();

	//// iOS specific ////

    /* For iOS only: Handle device rotation */
    void deviceRotated();
    
    /* For iOS only: Handle app going into background */
    void appWillResignActive();
    
    /* For iOS only: Handle app coming back from background */
    void appDidBecomeActive();
    
    /* For iOS only: Handle app being terminated */
    void appWillTerminate();
    
	/* Make renderer accessible to public so we can get the UIView on iOS */
    Renderer& getProjectRenderer()
    {
        return bRenderer();
    }
    
private:
	/* Helper Functions */
	GLfloat randomNumber(GLfloat min, GLfloat max);

	/* Variables */
	GLfloat _randomOffset;
	GLfloat _offset;
	bool _running = false;
	double _mouseX, _mouseY;
	GLint _lastStateSpaceKey = 0;
	vmml::Matrix4f _viewMatrixHUD;

};

#endif /* defined(PROJECT_MAIN_H) */
