#ifndef FREE_CAMERA_SCRIPT_H
#define FREE_CAMERA_SCRIPT_H

#include "Core.h"
#include "TheRenderer.h"
#include "Behaviour.h"
#include "TheTime.h"

class FreeCameraScript : public Behaviour {
public:
	float speed = 10.0f;
	double _mouseX, _mouseY;
	GLint _lastStateSpaceKey = bRenderer::INPUT_UNDEFINED;

	void Update() {
		//// Adjust aspect ratio ////
		TheRenderer::Instance()->renderer->getObjects()->getCamera("camera")->setAspectRatio(TheRenderer::Instance()->renderer->getView()->getAspectRatio());

		double deltaCameraY = 0.0;
		double deltaCameraX = 0.0;
		double cameraForward = 0.0;
		double cameraSideward = 0.0;

		/* iOS: control movement using touch screen */
		if (Input::isTouchDevice()) {

			// pause using double tap
			if (TheRenderer::Instance()->renderer->getInput()->doubleTapRecognized()) {
				*TheRenderer::Instance()->isRunning = !(*TheRenderer::Instance()->isRunning);
			}

			if (TheRenderer::Instance()->renderer->isRunning()) {
				// control using touch
				TouchMap touchMap = TheRenderer::Instance()->renderer->getInput()->getTouches();
				int i = 0;
				for (auto t = touchMap.begin(); t != touchMap.end(); ++t)
				{
					Touch touch = t->second;
					// If touch is in left half of the view: move around
					if (touch.startPositionX < TheRenderer::Instance()->renderer->getView()->getWidth() / 2) {
						cameraForward = -(touch.currentPositionY - touch.startPositionY) / 100;
						cameraSideward = (touch.currentPositionX - touch.startPositionX) / 100;

					}
					// if touch is in right half of the view: look around
					else
					{
						deltaCameraY = (touch.currentPositionX - touch.startPositionX) / 2000;
						deltaCameraX = (touch.currentPositionY - touch.startPositionY) / 2000;
					}
					if (++i > 2)
						break;
				}
			}

		}
		/* Windows: control movement using mouse and keyboard */
		else {
			// use space to pause and unpause
			GLint currentStateSpaceKey = TheRenderer::Instance()->renderer->getInput()->getKeyState(bRenderer::KEY_SPACE);
			if (currentStateSpaceKey != _lastStateSpaceKey)
			{
				_lastStateSpaceKey = currentStateSpaceKey;
				if (currentStateSpaceKey == bRenderer::INPUT_PRESS) {
					

					*TheRenderer::Instance()->isRunning = !(*TheRenderer::Instance()->isRunning);
					TheRenderer::Instance()->renderer->getInput()->setCursorEnabled(!TheRenderer::Instance()->renderer->isRunning());
				}
			}

			// mouse look
			double xpos, ypos; bool hasCursor = false;
			TheRenderer::Instance()->renderer->getInput()->getCursorPosition(&xpos, &ypos, &hasCursor);

			deltaCameraY = (xpos - _mouseX) / 1000;
			_mouseX = xpos;
			deltaCameraX = (ypos - _mouseY) / 1000;
			_mouseY = ypos;

			if (TheRenderer::Instance()->renderer->isRunning()) {
				// movement using wasd keys
				if (TheRenderer::Instance()->renderer->getInput()->getKeyState(bRenderer::KEY_W) == bRenderer::INPUT_PRESS)
					if (TheRenderer::Instance()->renderer->getInput()->getKeyState(bRenderer::KEY_LEFT_SHIFT) == bRenderer::INPUT_PRESS) 			cameraForward = 2.0;
					else			cameraForward = 1.0;
				else if (TheRenderer::Instance()->renderer->getInput()->getKeyState(bRenderer::KEY_S) == bRenderer::INPUT_PRESS)
					if (TheRenderer::Instance()->renderer->getInput()->getKeyState(bRenderer::KEY_LEFT_SHIFT) == bRenderer::INPUT_PRESS) 			cameraForward = -2.0;
					else			cameraForward = -1.0;
				else
					cameraForward = 0.0;

				if (TheRenderer::Instance()->renderer->getInput()->getKeyState(bRenderer::KEY_A) == bRenderer::INPUT_PRESS)
					cameraSideward = -1.0;
				else if (TheRenderer::Instance()->renderer->getInput()->getKeyState(bRenderer::KEY_D) == bRenderer::INPUT_PRESS)
					cameraSideward = 1.0;
				if (TheRenderer::Instance()->renderer->getInput()->getKeyState(bRenderer::KEY_UP) == bRenderer::INPUT_PRESS)
					TheRenderer::Instance()->renderer->getObjects()->getCamera("camera")->moveCameraUpward( speed *TheTime::deltaTime);
				else if (TheRenderer::Instance()->renderer->getInput()->getKeyState(bRenderer::KEY_DOWN) == bRenderer::INPUT_PRESS)
					TheRenderer::Instance()->renderer->getObjects()->getCamera("camera")->moveCameraUpward(- speed *TheTime::deltaTime);
				if (TheRenderer::Instance()->renderer->getInput()->getKeyState(bRenderer::KEY_LEFT) == bRenderer::INPUT_PRESS)
					TheRenderer::Instance()->renderer->getObjects()->getCamera("camera")->rotateCamera(0.0f, 0.0f, 0.03f* speed *TheTime::deltaTime);
				else if (TheRenderer::Instance()->renderer->getInput()->getKeyState(bRenderer::KEY_RIGHT) == bRenderer::INPUT_PRESS)
					TheRenderer::Instance()->renderer->getObjects()->getCamera("camera")->rotateCamera(0.0f, 0.0f, -0.03f*speed*TheTime::deltaTime);
			}
		}

		//// Update camera ////
		if (TheRenderer::Instance()->renderer->isRunning()) {
			TheRenderer::Instance()->renderer->getObjects()->getCamera("camera")->moveCameraForward(cameraForward * speed * TheTime::deltaTime);
			TheRenderer::Instance()->renderer->getObjects()->getCamera("camera")->rotateCamera(deltaCameraX, deltaCameraY, 0.0f);
			TheRenderer::Instance()->renderer->getObjects()->getCamera("camera")->moveCameraSideward(cameraSideward * speed * TheTime::deltaTime);
		}
	}
};

#endif /* defined(FREE_CAMERA_SCRIPT_H) */