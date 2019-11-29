#ifndef TEXTURE_H
#define TEXTURE_H

class Scene;

#include <string>

#include "PhotonBox/core/ManagedResource.h"
#include "PhotonBox/core/LazyLoadable.h"
#include "PhotonBox/core/OpenGL.h"
#include "PhotonBox/resource/ImageBuffer.h"

class Texture : public ManagedResource, public LazyLoadable, public ImageBuffer
{
public:
	struct Config {
		bool mips = false;
		bool hdr = false;
		int width = 0;
		int height = 0;
	};

	Texture(Config config = Config());
	Texture(Filepath filePath, Config config = Config());
	~Texture();

	void bind();
	void bind(unsigned int textureUnit) override;
	void enable() override {}
	int getWidth();
	int getHeight();
	bool isHDR();
	GLuint getTextureLocation() { return _texture; }

	void submitBuffer() override;
private:
	Config _config;

	GLuint _texture;
	unsigned char* _data = NULL;

	void setData(unsigned char* data);
	void loadFromFile() override;
	void blankInitialize();
};

#endif // TEXTURE_H
