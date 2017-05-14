#include "headers/OSdetect.h"

#ifdef B_OS_DESKTOP

#include "headers/TextureData.h"
#include "bRenderer.h"
#include "FreeImage.h"

TextureData &TextureData::load(const std::string &fileName)
{
	std::string fullFileName = bRenderer::getFilePath(fileName);

	static const size_t COMPONENTS = 4;
	static const size_t BITS_PER_COMPONENT = 8;

	_format = GL_RGBA;

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fullFileName.c_str(), 0); 
	FIBITMAP* imagen = FreeImage_Load(format, fullFileName.c_str());

	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);

	_width = FreeImage_GetWidth(imagen);
	_height = FreeImage_GetHeight(imagen);

	GLubyte* texture = new GLubyte[4 * _width*_height];
	char* pixeles = (char*)FreeImage_GetBits(imagen);

	//FreeImage loads in BGR format, so we need to swap some bytes (or use GL_BGR).
	for (int j = 0; j<_width*_height; j++){
		texture[j * 4 + 0] = pixeles[j * 4 + 2];
		texture[j * 4 + 1] = pixeles[j * 4 + 1];
		texture[j * 4 + 2] = pixeles[j * 4 + 0];
		texture[j * 4 + 3] = pixeles[j * 4 + 3];
	}

	_imageData = ImageDataPtr(texture);

	return *this;
}

#endif