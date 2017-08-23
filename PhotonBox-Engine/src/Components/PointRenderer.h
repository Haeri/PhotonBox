#ifndef POINT_RENDERER_H
#define POINT_RENDERER_H

#include "ObjectRenderer.h"
#include "../Core/Display.h"
#include "Transform.h"
#include "Camera.h"

class PointRenderer : public ObjectRenderer {
public:
	void init() override {}
	void render() override{
		glDepthFunc(GL_ALWAYS);
		glPointSize(10.0);

		glUseProgram(0);
		glBegin(GL_POINTS);
		glColor4f(1, 0, 0, 1);

		Vector4f vec = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix() * Vector4f(transform->getPosition(), 1);
		Vector3f vec2 = Vector3f(vec.x(), vec.y(), vec.z()) / vec.w();
		

		float winx = ((vec2.x() + 1.0) / 2.0) * Display::getWidth();
		float winy = ((1.0 - vec2.y()) / 2.0) *  Display::getHeight();

		glVertex2f(winx, winy);
		
		glEnd(); 
		glFinish();

		glDepthFunc(GL_LESS);

	}
};

#endif // POINT_RENDERER_H
