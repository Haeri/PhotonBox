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
		glPointSize(6.0);

		glBegin(GL_POINTS);
		glColor4f(1, 0, 0, 1);

		//clip_position = Projection_Matrix * Modelview_Matrix * vertex_position
		
		Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		Vector3f vec = Vector3f(mvp(0, 3), mvp(1, 3), mvp(2, 3)) / mvp(3, 3);
		
		float winx = ((vec.x() + 1.0) / 2.0) * Display::getWidth();
		float winy = ((1.0 - vec.y()) / 2.0) *  Display::getHeight();

		glVertex2f(winx, winy);
		
		glEnd(); glFinish();

		glDepthFunc(GL_LESS);
	}
};

#endif // POINT_RENDERER_H
