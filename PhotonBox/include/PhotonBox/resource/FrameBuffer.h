#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

class Material;

#include <unordered_map>
#include <vector>
#include <map>
#include <string>

#include "PhotonBox/core/OpenGL.h"
#include "PhotonBox/resource/ImageBuffer.h"

#include "PhotonBox/PhotonBoxConfig.h"

class PB_PORT FrameBuffer
{
public:

	static const std::string DEFAULT_ATTACHMENT;

	struct BufferAttachment : public ImageBuffer
	{
		std::string name;
		bool hdr;
		int mipmaps;
		GLuint id;
		GLuint attachmentIndex = -1;
		FrameBuffer* frameBuffer;

		BufferAttachment() {}
		BufferAttachment(FrameBuffer* frameBuffer, std::string name, bool hdr)
			:frameBuffer(frameBuffer), name(name), hdr(hdr)
		{
			id = 0;
			attachmentIndex = 0;
			mipmaps = 0;
		}

		void enable() override {}
		void bind(unsigned int textureUnit) override 
		{
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, id);
			if (mipmaps > 0)
				glGenerateMipmap(GL_TEXTURE_2D);
		}
	};

	enum InterpolationType
	{
		NEAREST, 
		LINEAR,
		LINEAR_NEAREST,
		NEAREST_LINEAR
	};

	enum EdgeType
	{
		CLAMP_TO_EDGE, 
		CLAMP_TO_BORDER, 
		MIRRORED_REPEAT, 
		REPEAT, 
		MIRROR_CLAMP_TO_EDGE
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

	void addTextureAttachment(std::string name, bool hdr = false, bool mipmaps = false, InterpolationType interpolationType = LINEAR, EdgeType edgeType = CLAMP_TO_EDGE);
	void addDepthTextureAttachment(std::string name);
	void addDepthBufferAttachment();
	BufferAttachment* getAttachment(std::string name);

	/// <summary>
	/// Enables this FrameBuffer to be drawn to.
	/// </summary>
	void enable();

	/// <summary>
	/// Binds the specified texture unit.
	/// </summary>
	/// <param name="textureUnit">The texture unit.</param>
	/// <param name="name">The name of the attachment to be bound.</param>
	void bind(unsigned int textureUnit, std::string name);

	/// <summary>
	/// Readies this instance. Call after all Attachments are added.
	/// </summary>
	void ready();

	/// <summary>
	/// Clears this instance.
	/// </summary>
	void clear(float r = 0, float g = 0, float b = 0, float a = 0);
	void render(std::string name);
	void render(Material* material);
	void resize();

	void blit(FrameBuffer* target, std::string sourceAttachment, std::string targetAttachment);
	void blitToScreen(std::string name);
	GLuint getTextureID(std::string name) { return _colorAttachments[name].id; }
	int getWidth() { return _width; }
	int getHeight() { return _height; }
	GLuint getFBO() { return _fbo; }

	static void resetDefaultBuffer();
	static void clearDefaultBuffer();
	static void resizeAll();
	static void destroy();

private:

	// config
	int _width, _height;
	float _screenFactor;

	// Buffers
	GLuint _fbo;
	GLuint _depthAttachment = -1;
	std::unordered_map<std::string, BufferAttachment> _colorAttachments;

	GLenum _colorAttachmentIndex = GL_COLOR_ATTACHMENT0;
	std::vector<GLenum> _drawBuffers;

	//Opengl Wrapper
	static std::map<InterpolationType, GLint> _interpolationTypes;
	static std::map<InterpolationType, GLint> _interpolationMipTypes;
	static std::map<EdgeType, GLint> _edgeTypes;

	// Mesh
	static GLuint _quadVAO;
	static GLuint _quadVBO;

	static GLuint _currentFBO;
	static std::vector<FrameBuffer*> _bufferList;

	void render(std::string name, Material* material);
	void initialize();

};

#endif //FRAME_BUFFER_H
