#ifndef B_RENDERER_H
#define B_RENDERER_H

/* bRenderer includes */
#include "OSdetect.h"
#include "Renderer_GL.h"
#include "Logger.h"
#include "View.h"
#include "Input.h"
#include "ObjectManager.h"
#include "ModelRenderer.h"

#ifdef __OBJC__
@class RendererCaller;
#endif

class IRenderProject;

/** @brief The main class that is able to initialize and maintain everything that is necessary to render an image to the screen
*	@author Benjamin Buergisser
*/
class Renderer
{
public:
    
#ifndef __OBJC__
    typedef int RendererCaller;
#endif

	/* Functions */

	/**	@brief Constructor
	*/
	Renderer();

	/**	@brief Virtual destructor
	*/
	virtual ~Renderer(){}

	/**	@brief Returns a pointer to the view of the renderer
	*
	*	This function returns a pointer to the view of the renderer. The view is where the OpenGL context and
	*	on desktop systems a window is created and managed. On iOS a UIView is created instead.
	*
	*/
	ViewPtr getView();

	/**	@brief Returns a pointer to the input handler of the renderer
	*/
	InputPtr getInput();

	/**	@brief Returns a pointer to the object management of the renderer
	*/
	ObjectManagerPtr getObjects();

	/**	@brief Returns a pointer to the model renderer
	*/
	ModelRendererPtr getModelRenderer();

	/**	@brief Returns true if the renderer has already been initialized
	*/
	bool isInitialized();

	/**	@brief Returns true if the renderer is running
	*/
	bool isRunning();

	/**	@brief Sets the project instance to be used for function calls
	*
	*	The init, loop and terminate function of the set instance get called automatically.
	*
	*	@param[in] p The project instance
	*/
	void setRenderProject(IRenderProject *p);

	/**	@brief Sets a static function that gets called when initializing the renderer
	*
	*	In an application a function can be defined that gets called immediately after the renderer is initialized.
	*
	*	@param[in] f The function to be called
	*/
	void setInitFunction(void(*f)());

	/**	@brief Sets a static function that gets called repeatedly when looping
	*
	*	The set function gets called repeatedly while the renderer is running.
	*	Usually the scene gets drawn and setup in the defined function.
	*
	*	@param[in] f The function to be called
	*/
	void setLoopFunction(void(*f)(const double deltaTime, const double elapsedTime));

	/**	@brief Sets a static function that gets called when terminating the renderer
	*
	*	The set function gets called when the renderer is terminated.
	*	Here allocated memory can be freed, instances deleted and statuses changed.
	*
	*	@param[in] f The function to be called
	*/
	void setTerminateFunction(void(*f)());

	/**	@brief Do all necessary initializations for the renderer to be ready to run (returns true in the case of success)
	*	@param[in] windowTitle The title for the window on desktop systems (optional)
	*/
	bool initRenderer(std::string windowTitle = bRenderer::DEFAULT_WINDOW_TITLE());

	/**	@brief Do all necessary initializations for the renderer to be ready to run (returns true in the case of success)
	*	@param[in] fullscreen Decides whether or not the application runs in full screen mode
	*	@param[in] windowTitle The title for the window on desktop systems (optional)
	*/
	bool initRenderer(bool fullscreen, std::string windowTitle = bRenderer::DEFAULT_WINDOW_TITLE());

	/**	@brief Do all necessary initializations for the renderer to be ready to run (returns true in the case of success)
	*	@param[in] width The width of the window in pixels
	*	@param[in] height The height of the window in pixels
	*	@param[in] fullscreen Decides whether or not the application runs in full screen mode (optional)
	*	@param[in] windowTitle The title for the window on desktop systems (optional)
	*/
	bool initRenderer(GLint width, GLint height, bool fullscreen = false, std::string windowTitle = bRenderer::DEFAULT_WINDOW_TITLE());

	/**	@brief Start the render loop
	*/
	void runRenderer();

	/**	@brief Stop the render loop
	*/
	void stopRenderer();

	/**	@brief Terminate the renderer
	*/
	void terminateRenderer();

private:
	/* Functions */

    /**	@brief Update the scene
     */
    void update();
    
	/**	@brief Reset all variables
	*/
	void reset();

	/* Variables */

	bool _initialized = false;
	bool _running = false;

	double _elapsedTime = 0.0, _stopTime = 0.0, _startTime = 0.0;

	ViewPtr				_view = nullptr;
	InputPtr			_input = nullptr;
	ObjectManagerPtr	_objectManager = nullptr;
	ModelRendererPtr	_modelRenderer = nullptr;

	IRenderProject *_renderProject = nullptr;
    
	RendererCaller *_rendererCaller = nullptr;
	
	void(*_initFunction)() = nullptr;
	void(*_loopFunction)(const double deltaTime, const double elapsedTime) = nullptr;
	void(*_terminateFunction)() = nullptr;

};

typedef std::shared_ptr< Renderer >  RendererPtr;

#endif /* defined(B_RENDERER_H) */
