#ifndef AXIS_RENDERER_H
#define AXIS_RENDERER_H

#include "ObjectRenderer.h"
#include "../Core/Display.h"
#include "Transform.h"
#include "Camera.h"

class AxisRenderer : public ObjectRenderer {
public:
	GLuint axisVAO_;

	void init() override {
		createAxis();
	}

	void render() override {
		glBindVertexArray(axisVAO_);

		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

	void createAxis()
	{
		std::vector<float> positions{ 0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f };
		std::vector<float> colors{ 1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f };

		glGenVertexArrays(1, &axisVAO_);
		glBindVertexArray(axisVAO_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		std::vector<GLuint> bufferIds;
		glGenBuffers(3, bufferIds.data());

		glBindBuffer(GL_ARRAY_BUFFER, bufferIds[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions),
			positions.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(VPosAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VPosAttribLoc);

		glBindBuffer(GL_ARRAY_BUFFER, bufferIds[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors),
			colors.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(VColAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VColAttribLoc);

		glBindBuffer(GL_ARRAY_BUFFER, bufferIds[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions),
			positions.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(VNormAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VNormAttribLoc);

		// Unbind objects
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(3, bufferIds.data());
	}

	void onDestroy() override {
	}
private:
	const GLuint VPosAttribLoc = GLuint(0);
	const GLuint VColAttribLoc = GLuint(1);
	const GLuint VNormAttribLoc = GLuint(2);
	const GLuint VTexCoordAttribLoc = GLuint(3);
};

#endif // AXIS_RENDERER_H







