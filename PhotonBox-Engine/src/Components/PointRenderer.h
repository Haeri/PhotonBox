#ifndef POINT_RENDERER_H
#define POINT_RENDERER_H

#include "ObjectRenderer.h"
#include "../Core/Display.h"
#include "Transform.h"
#include "Camera.h"
#include "../Core/GameObject.h"

class PointRenderer : public ObjectRenderer {
public:

	void render() override{

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPointSize(10.0);

		//glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);

		glUseProgram(0);
		glBegin(GL_POINTS);

			Vector3f col = gameObject->getComponent<PointLight>()->color;
			glColor4f(col.x(), col.y(), col.z(), 1);

			Vector2f pos2D = Camera::worldToScreen(transform->getPositionWorld());
			glVertex2f(pos2D.x(), pos2D.y());
		
		glEnd(); 
		glFinish();

		glDepthFunc(GL_LESS);
	}

	float getBoundingSphereRadius() { return 1; }
};

#endif // POINT_RENDERER_H
