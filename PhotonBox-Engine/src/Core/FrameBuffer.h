#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

class Material;
#include "./Display.h"

class FrameBuffer {
public:
	FrameBuffer(int width, int height);
	void enable();
	void render();
	void render(Material* material);
	void destroy();
	GLuint getTextureID() { return _texColor; }
	int getWidth() { return _width; }
	int getHeight() { return _height; }

	static void resetDefaultBuffer();
private:
	// Buffers
	int _width, _height;
	GLuint _fbo;
	GLuint _rbDS;
	GLuint _texColor;

	// Mesh
	GLuint _quadVAO;
	GLuint _quadVBO;

	static GLuint _currentFBO;
};

#endif //FRAME_BUFFER_H