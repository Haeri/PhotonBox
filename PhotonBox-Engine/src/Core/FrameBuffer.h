#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

class Material;
#include "./Display.h"

class FrameBuffer {
public:
	FrameBuffer(int width, int height);
	FrameBuffer(int width, int height, bool mipmaps);
	FrameBuffer(int width, int height, bool mipmaps, bool hdr);
	void enable();
	void bind();
	void bind(GLuint textureUnit);
	void clear();
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
	GLint _isHDR;
	GLuint _fbo;
	GLuint _rbDS;
	GLuint _texColor;

	// Mesh
	GLuint _quadVAO;
	GLuint _quadVBO;

	int _maxMipMaps;

	static GLuint _currentFBO;
};

#endif //FRAME_BUFFER_H