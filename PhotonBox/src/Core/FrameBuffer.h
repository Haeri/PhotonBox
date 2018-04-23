#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

class Material;

#include <unordered_map>
#include <vector>
#include "OpenGL.h"

class FrameBuffer
{
public:
	struct BufferAttachment
	{
		std::string name;
		bool hdr;
		int mipmaps;
		GLuint id;
		GLuint attachmentIndex;
		FrameBuffer* frameBuffer;

		BufferAttachment() {}
		BufferAttachment(FrameBuffer* frameBuffer, std::string name, bool hdr) : frameBuffer(frameBuffer), name(name), hdr(hdr)
		{
			id = 0;
			attachmentIndex = 0;
			mipmaps = 0;
		}
	};
	/// <summary>
	/// Initializes a new instance of the <see cref="FrameBuffer"/> class.
	/// </summary>
	/// <param name="screenFactor">The resolutiin factor bound to the screen.</param>
	FrameBuffer(float screenFactor);
	/// <summary>
	/// Initializes a new instance of the <see cref="FrameBuffer"/> class.
	/// </summary>
	/// <param name="width">The fixed width of the FrameBuffer.</param>
	/// <param name="height">The fixed height of the FrameBuffer.</param>
	FrameBuffer(int width, int height);
	~FrameBuffer();
	void addTextureAttachment(std::string name);
	void addTextureAttachment(std::string name, bool hdr);
	void addTextureAttachment(std::string name, bool hdr, bool mipmaps);
	void addDepthTextureAttachment(std::string name);
	void addDepthBufferAttachment();

	/// <summary>
	/// Enables this FrameBuffer to be drawn to.
	/// </summary>
	void enable();

	/// <summary>
	/// Binds the specified texture unit.
	/// </summary>
	/// <param name="textureUnit">The texture unit.</param>
	/// <param name="name">The name of the attachment to be bound.</param>
	void bind(GLuint textureUnit, std::string name);

	/// <summary>
	/// Readies this instance. Call after all Attachments are added.
	/// </summary>
	void ready();

	/// <summary>
	/// Clears this instance.
	/// </summary>
	void clear(int r = 0, int g = 0, int b = 0, int a = 0);
	void render(std::string name);
	void render(Material* material);
	void resize();

	BufferAttachment* getAttachment(std::string name);
	GLuint getTextureID(std::string name) { return _colorAttachments[name].id; }
	int getWidth() { return _width; }
	int getHeight() { return _height; }

	static void resetDefaultBuffer();
	static void resizeAll();
	GLuint _fbo;
private:
	// config
	int _width, _height;
	float _screenFactor;

	// Buffers
	GLuint _depthAttachment = -1;
	std::unordered_map<std::string, BufferAttachment> _colorAttachments;

	GLenum _colorAttachmentIndex = GL_COLOR_ATTACHMENT0;
	std::vector<GLenum> _drawBuffers;

	// Mesh
	static GLuint _quadVAO;
	GLuint _quadVBO;

	static GLuint _currentFBO;
	static std::vector<FrameBuffer*> _bufferList;

	void render(std::string name, Material* material);
	void initialize();
};

#endif //FRAME_BUFFER_H
