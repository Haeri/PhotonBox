#ifndef TEXTURE_SERIALIZER_H
#define TEXTURE_SERIALIZER_H

#include <string>

/*
*	Extension: .pbt
*
*	Format:
*
*	Name			Type			Byte size		Description
*	----------------------------------------------------------------------------------
*	version			short			2				Version of the serialization file
*	buff_size		size_t			8				Size of the uncompressed buffer
*	cp_buff_size	size_t			8				Size of the compressed buffer
*	buff			unsigned char*	{cp_buff_size}	Buffer containing zlib compressed data		
*
*
*	Buffer content:
*
*	Name			Type			Byte size		Description
*	----------------------------------------------------------------------------------
*	width			int				4				Width of image
*	height			int				4				Height of image
*	components		int				4				Amount of components per pixel (rgb, rgba, ...)
*	data			unsigned char*	{w*h*c}			Image data
*
*/


class TextureSerializer
{
public:
	static const short SERIALIZER_VERSION;
	static const std::string EXTENSION;

	// TODO: Change input arguments to struct
	static void write(std::string name, int width, int height, int components, unsigned char* data);
	static unsigned char* read(std::string name, int* width, int* height, int* components);
	static void free_buffer(unsigned char* data);
};

#endif // TEXTURE_SERIALIZER_H
