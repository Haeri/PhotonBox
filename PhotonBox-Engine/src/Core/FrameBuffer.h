#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <iostream>
#include "./Display.h"
#include "../Resources/Material.h"
#include "../Resources/Vertex.h"

class FrameBuffer {
public:
	FrameBuffer(int width, int height) : _width(width), _height(height) {

		// Create framebuffer
		glGenFramebuffers(1, &_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		// Create texture to hold color buffer
		glGenTextures(1, &_texColor);
		glBindTexture(GL_TEXTURE_2D, _texColor);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texColor, 0);

		// Create Renderbuffer Object to hold depth and stencil buffers
		glGenRenderbuffers(1, &_rbDS);
		glBindRenderbuffer(GL_RENDERBUFFER, _rbDS);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbDS);



		// Error check
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cerr << "Error while createing framebuffer" << std::endl;



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

	void enable() {
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glViewport(0, 0, _width, _height);
	}

	void render(Material* material){
		render(material, 987654);
	}

	void render(Material* material, GLuint second) {
		glBindVertexArray(_quadVAO);

		material->shader->bind();

		material->shader->update(nullptr);
		material->updateUniforms();
		//material->bindTextures();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texColor);
		if (second != 987654) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, second);
		}
		/*
		glUniform1i(_texColor, 0);
		if (second != 987654) {
			glUniform1i(second, 1);
		}
		*/
		material->shader->updateTextures();

		material->shader->enableAttributes();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		material->shader->disableAttributes();

		glBindVertexArray(0);
	}
	
	void destroy() {
		glDeleteFramebuffers(1, &_fbo);
		glDeleteRenderbuffers(1, &_rbDS);

	}

	GLuint getTextureID() {
		return _texColor;
	}

	static void resetDefaultBuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Display::getWidth(), Display::getHeight());
	}

private:
	// Buffers
	int _width, _height;
	GLuint _fbo;
	GLuint _rbDS;
	GLuint _texColor;

	// Mesh
	GLuint _quadVAO;
	GLuint _quadVBO;
};

#endif //FRAME_BUFFER_H
