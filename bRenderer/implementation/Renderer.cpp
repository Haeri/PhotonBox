#include "headers/Renderer.h"
#include "headers/IRenderProject.h"
#include "headers/Configuration.h"

/* Public functions */

Renderer::Renderer()
{
	reset();
}

ViewPtr Renderer::getView()
{
	return _view;
}

InputPtr Renderer::getInput()
{
	return _input;
}

ObjectManagerPtr Renderer::getObjects()
{
	return _objectManager;
}

ModelRendererPtr Renderer::getModelRenderer()
{
	return _modelRenderer;
}

bool Renderer::isInitialized()
{
	return _initialized;
}

bool Renderer::isRunning()
{
    return _running;
}

void Renderer::setRenderProject(IRenderProject *p)
{
	_renderProject = p;
}

void Renderer::setInitFunction(void(*f)())
{
	_initFunction = f;
}

void Renderer::setLoopFunction(void(*f)(const double deltaTime, const double elapsedTime))
{
	_loopFunction = f;
}

void Renderer::setTerminateFunction(void(*f)())
{
	_terminateFunction = f;
}

bool Renderer::initRenderer(std::string windowTitle)
{
    if (_initialized)
        return true;
    
    // initialize view
    if(!_view)  _view = ViewPtr(new View);
	if (!_view->initView(windowTitle))
        return false;
  
	// Input
    if(!_input)  			_input = InputPtr(new Input(_view));

	// Object management
	if (!_objectManager)  _objectManager = ObjectManagerPtr(new ObjectManager);

	// Render queue
	if (!_modelRenderer)  _modelRenderer = ModelRendererPtr(new ModelRenderer(_objectManager));

    // call static function if set
    if (_initFunction)
        _initFunction();
    
    // call member function if set
    if (_renderProject)
        _renderProject->initFunction();

	_initialized = true;
    
    return true;
}

bool Renderer::initRenderer(bool fullscreen, std::string windowTitle)
{
    if (_initialized)
        return true;
    
    // initialize view
    if(!_view)  _view = ViewPtr(new View);
	if (!_view->initView(fullscreen, windowTitle))
        return false;
    
	return initRenderer(windowTitle);
}

bool Renderer::initRenderer(GLint width, GLint height, bool fullscreen, std::string windowTitle)
{
    if (_initialized)
        return true;
    
    // initialize view
    if(!_view) 		_view = ViewPtr(new View);
	if (!_view->initView(width, height, fullscreen, windowTitle))
        return false;
    
	return initRenderer(windowTitle);
}

/* Private functions */

void Renderer::reset()
{
	_initialized = false;
	_running = false;

	_stopTime = 0;
	_startTime = 0;
	_elapsedTime = 0;

	if (_objectManager)
		_objectManager->clear(true);
}