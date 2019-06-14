#ifndef TEXTURE_H
#define TEXTURE_H

class Scene;

#include <string>

#include "PhotonBox/core/ManagedResource.h"
#include "PhotonBox/core/ILazyLoadable.h"
#include "PhotonBox/core/OpenGL.h"
#include "PhotonBox/resource/ImageBuffer.h"

class Texture : public ManagedResource, public ILazyLoadable, public ImageBuffer
{
public:
	Texture(std::string filePath, bool generateMipMaps = false, bool hdr = false);
	~Texture();

	void bind();
	void bind(unsigned int textureUnit) override;
	void enable() override {}
	int getWidth() { return _width; }
	int getHeight() { return _height; }
	bool isHDR() { return _isHDR; }
	GLuint getTextureLocation() { return _texture; }

	void submitBuffer() override;
private:
	friend class Scene;
	bool _isHDR, _isMipMap;
	GLuint _texture;
	int _width, _height;
	unsigned char* _data = NULL;

	void loadFromFile() override;
	void blankInitialize();
};

#endif // TEXTURE_H
