#ifndef POINT_RENDERER_H
#define POINT_RENDERER_H

#include "ObjectRenderer.h"
#include "../Core/Display.h"
#include "Transform.h"
#include "Camera.h"

class PointRenderer : public ObjectRenderer {
public:

	void render() override{

		glLoadIdentity();
		glPointSize(10.0);

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
};

#endif // POINT_RENDERER_H
