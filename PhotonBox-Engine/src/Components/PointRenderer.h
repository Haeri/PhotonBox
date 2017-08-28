#ifndef POINT_RENDERER_H
#define POINT_RENDERER_H

#include "ObjectRenderer.h"
#include "../Core/Display.h"
#include "Transform.h"
#include "Camera.h"

class PointRenderer : public ObjectRenderer {
public:
	Texture* texture;

	void init() override {
		texture = new Texture("./res/trooper.png");
	}


	void render() override{
		glDepthFunc(GL_ALWAYS);
		glPointSize(10.0);

		glUseProgram(0);
		glBegin(GL_POINTS);
		texture->bind();
		glColor4f(1, 0, 0, 1);

		Vector2f pos2D = Camera::worldToScreen(transform->getPositionWorld());
		glVertex2f(pos2D.x(), pos2D.x());
		
		glEnd(); 
		glFinish();

		glDepthFunc(GL_LESS);

	}

	void onDestroy() override {
		delete texture;
	}
};

#endif // POINT_RENDERER_H
