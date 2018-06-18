#ifndef POINT_RENDERER_H
#define POINT_RENDERER_H

#include "PhotonBox/core/Entity.h"
#include "PhotonBox/core/OpenGL.h"
#include "PhotonBox/components/Camera.h"
#include "PhotonBox/components/ObjectRenderer.h"
#include "PhotonBox/components/Transform.h"
#include "PhotonBox/resources/CircleShader.h"

class PointRenderer : public ObjectRenderer
{
public:
	GLuint _quadVAO = -1, _quadVBO = -1;
	void render() override
	{
		// Create mesh
		if (_quadVAO == -1)
		{
			// Quad Mesh
			static const GLfloat _quadVertices[] = {
				-1.0f,  1.0f,
				-1.0f, -1.0f,
				1.0f,  1.0f,
				1.0f, -1.0f,
			};

			glGenVertexArrays(1, &_quadVAO);
			glBindVertexArray(_quadVAO);

			glGenBuffers(1, &_quadVBO);
			glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertices), _quadVertices, GL_STATIC_DRAW);


			glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
			glVertexAttribPointer(Vertex::AttibLocation::POSITION, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		glBindVertexArray(_quadVAO);

		Shader* shader = CircleShader::getInstance();;

		shader->bind();
		shader->update(nullptr);

		shader->updateTextures();
		shader->enableAttributes();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		Renderer::addDrawCall();
		shader->disableAttributes();

		glBindVertexArray(0);

		/*
		//TODO: Rewrite with modern opengl
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPointSize(10.0);

		//glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);

		glUseProgram(0);
		glBegin(GL_POINTS);

		Vector3f col = entity->getComponent<PointLight>()->color;
		glColor4f(col.x(), col.y(), col.z(), 1);

		Vector2f pos2D = Camera::worldToScreen(transform->getPositionWorld());
		glVertex2f(pos2D.x(), pos2D.y());

		glEnd();
		glFinish();

		glDepthFunc(GL_LESS);
		*/
	}
};

#endif // POINT_RENDERER_H
