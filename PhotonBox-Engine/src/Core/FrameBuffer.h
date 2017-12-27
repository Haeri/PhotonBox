#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

class Material;
#include <unordered_map>
#include <vector>
#include "./Display.h"

class FrameBuffer {
public:
	static GLuint _quadVAO;

	FrameBuffer(int width, int height);
	//FrameBuffer(int width, int height, bool mipmaps);
	//FrameBuffer(int width, int height, bool mipmaps, bool hdr);
	void addTextureAttachment(std::string name);
	void addTextureAttachment(std::string name, bool hdr, bool drawBuffer);
	void addTextureAttachment(std::string name, bool hdr, bool drawBuffer, bool mipmaps);
	void addDepthTextureAttachment(std::string name);
	void addDepthTextureAttachment(std::string name, bool drawBuffer);
	void addDepthBufferAttachment();
	void enable();
	//void bind();
	//void bind(GLuint textureUnit);
	void bind(GLuint textureUnit, std::string name);
	void finish();
	void clear();
	void render(std::string name);
	void render(std::string name, Material* material);
	static void render(GLuint texId);
	void destroy();
	GLuint getTextureID(std::string name) { return _colorAttachments[name]; }
	int getWidth() { return _width; }
	int getHeight() { return _height; }

	static void resetDefaultBuffer();
private:
	// config
	int _width, _height;
	//bool _isHDR;

	// Buffers
	GLuint _fbo;
	//GLuint _rbDS;
	//GLuint _texColor;

	std::unordered_map<std::string, GLuint> _colorAttachments;
	GLuint _depthAttachment;

	GLenum _colorAttachmentIndex = GL_COLOR_ATTACHMENT0;
	std::vector<GLenum> _drawBuffers;


	// Mesh
	GLuint _quadVBO;

	int _maxMipMaps;

	static GLuint _currentFBO;
};

#endif //FRAME_BUFFER_H