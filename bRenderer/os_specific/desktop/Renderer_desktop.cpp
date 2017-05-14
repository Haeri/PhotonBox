#include "headers/OSdetect.h"

#ifdef B_OS_DESKTOP

#include "headers/Renderer.h"
#include "headers/IRenderProject.h"

/* Public functions */

void Renderer::runRenderer()
{
	_running = true;
	bRenderer::log("Renderer started", bRenderer::LM_SYS);

	_startTime += (glfwGetTime() - _startTime) - _stopTime;

	// loop until the user closes the window
	while (_running && _view->isRunning())
	{
		update();

		// poll for and process events
		glfwPollEvents();
	}

	if (!_view->isRunning())
		terminateRenderer();
}

void Renderer::stopRenderer()
{
	_stopTime = (glfwGetTime() - _startTime);
	_running = false;
	bRenderer::log("Renderer stopped", bRenderer::LM_SYS);
}

void Renderer::terminateRenderer()
{
    _view->setContextCurrent();
	reset();

	if (_terminateFunction)
		_terminateFunction();

	if (_renderProject)
		_renderProject->terminateFunction();

	_view->terminateView();
	glfwTerminate();

	bRenderer::log("Renderer terminated", bRenderer::LM_SYS);
}

/* Private functions */

void Renderer::update()
{
    // get time
    double currentTime = (glfwGetTime() - _startTime);
    
	_view->setContextCurrent();

	// clear
	_view->clearScreen();

	// render here
	if (_loopFunction)
		_loopFunction(currentTime - _elapsedTime, currentTime);

	if (_renderProject)
		_renderProject->loopFunction(currentTime - _elapsedTime, currentTime);

	// adjust time
	_elapsedTime = currentTime;

	// display
	_view->presentBuffer();
}

#endif