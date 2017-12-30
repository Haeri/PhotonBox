#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

class Material;
#include <unordered_map>
#include <vector>
#include "./Display.h"

class FrameBuffer {
public:
	struct BufferAttachment {
		std::string name;
		bool hdr;
		int mipmaps;
		GLuint id;
		GLuint attachmentIndex;
		FrameBuffer* frameBuffer;

		BufferAttachment() {}
		BufferAttachment(FrameBuffer* frameBuffer, std::string name, bool hdr) : frameBuffer(frameBuffer), name(name), hdr(hdr) {
			id = 0;
			attachmentIndex = 0;
			mipmaps = 0;
		}
	};

	FrameBuffer(int width, int height);
	~FrameBuffer();
	void addTextureAttachment(std::string name);
	void addTextureAttachment(std::string name, bool hdr);
	void addTextureAttachment(std::string name, bool hdr, bool mipmaps);
	void addDepthTextureAttachment(std::string name);
	void addDepthBufferAttachment();
	void enable();
	void bind(GLuint textureUnit, std::string name);
	void ready();
	void clear();
	void render(std::string name);
	void render(Material* material);

	BufferAttachment* getAttachment(std::string name);
	GLuint getTextureID(std::string name) { return _colorAttachments[name].id; }
	int getWidth() { return _width; }
	int getHeight() { return _height; }

	static void resetDefaultBuffer();
private:
	// config
	int _width, _height;

	// Buffers
	GLuint _fbo;
	GLuint _depthAttachment;
	std::unordered_map<std::string, BufferAttachment> _colorAttachments;

	GLenum _colorAttachmentIndex = GL_COLOR_ATTACHMENT0;
	std::vector<GLenum> _drawBuffers;

	// Mesh
	static GLuint _quadVAO;
	GLuint _quadVBO;

	static GLuint _currentFBO;

	void render(std::string name, Material* material);
};

#endif //FRAME_BUFFER_H