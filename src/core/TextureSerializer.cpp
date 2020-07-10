#include "PhotonBox/core/TextureSerializer.h"

#include <iostream>
#include <fstream>
#include <string.h>

#include "PhotonBox/util/Util.h"
#include "PhotonBox/util/Logger.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

const short TextureSerializer::SERIALIZER_VERSION = 1;
const std::string TextureSerializer::EXTENSION = ".pbt";

void TextureSerializer::write(std::string name, int width, int height, int components, unsigned char* data)
{
	std::fstream f = std::fstream(name, std::ios::out | std::ios::binary);
	if (f.is_open())
	{
		size_t dataSize = (width * height * components) * sizeof(unsigned char);
		size_t buff_size = sizeof(int) + sizeof(int) + sizeof(int) + dataSize;
		size_t cp_buff_size = 0;

		// Create one buffer from all the components
		unsigned char* buff = Util::combineBuffer(
			8,
			(unsigned char*)&width, sizeof(int),
			(unsigned char*)&height, sizeof(int),
			(unsigned char*)&components, sizeof(int),
			data, dataSize
		);


		unsigned char* curr = buff;

		int re_width = *reinterpret_cast<int*>(curr);
		curr += sizeof(int);
		int re_height = *reinterpret_cast<int*>(curr);
		curr += sizeof(int);
		int re_components = *reinterpret_cast<int*>(curr);
		curr += sizeof(int);


		if (width != re_width ||
			height != re_height ||
			components != re_components)
		{
			// Something went wrong while combining components to one buffer
			Logger::errln("Buffer combination failed!");
		}



		unsigned char* cp_buff = Util::compress(buff, buff_size, &cp_buff_size);

		f.seekg(0);
		f.write((char*)&SERIALIZER_VERSION, sizeof(short));
		f.write((char*)&buff_size, sizeof(size_t));
		f.write((char*)&cp_buff_size, sizeof(size_t));
		f.write((char*)cp_buff, cp_buff_size);
		f.close();

		free_buffer(buff);
		free_buffer(cp_buff);
	}
	else
	{
		Logger::errln("Could not create", name);
	}
}

unsigned char* TextureSerializer::read(std::string name, int* width, int* height, int* components)
{
	std::fstream f = std::fstream(name, std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		Logger::errln("Could not read", name);
		return nullptr;
	}

	size_t dataSize = -1;
	size_t buff_size = -1;
	size_t cp_buff_size = -1;
	short version = -1;
	unsigned char* cp_buff;

	f.seekg(0);
	
	if (!f.read((char*)&version, sizeof(short)))
	{
		Logger::errln("Could not read version in file", name);
		return nullptr;
	}

	if (version != SERIALIZER_VERSION) 
	{
		Logger::errln("Old or unsupported Texture version!");
		return nullptr;
	}

	if(!f.read((char*)&buff_size, sizeof(size_t)))
	{
		Logger::errln("Could not read buffer_size in file", name);
		return nullptr;
	}

	if (!f.read((char*)&cp_buff_size, sizeof(size_t)))
	{
		Logger::errln("Could not read compressed_buffer_size in file", name);
		return nullptr;
	}

	cp_buff = (unsigned char*)malloc(cp_buff_size);
		
	if (!f.read((char*)cp_buff, cp_buff_size))
	{
		Logger::errln("Could not read compressed_buffer in file", name);
		return nullptr;
	}
	f.close();

	unsigned char* buff = Util::decompress(cp_buff, cp_buff_size, buff_size);
	unsigned char* curr = buff;

	*width = *reinterpret_cast<int*>(curr);
	curr += sizeof(int);
	*height = *reinterpret_cast<int*>(curr);
	curr += sizeof(int);
	*components = *reinterpret_cast<int*>(curr);
	curr += sizeof(int);

	if (*width < 0 || *height < 0 || *components < 0)
	{
		// Looks like thexture was incorrectly inflated
		Logger::errln("Texture was inflated incorrectly!");
		return nullptr;
	}


	dataSize = (*width * *height * *components) * sizeof(unsigned char);
	unsigned char* data = (unsigned char*)malloc(dataSize);
	if(data == 0)
	{
		// Looks like thexture was incorrectly inflated
		Logger::errln("Unable to allocate data");
		return nullptr;
	}
	
	memcpy(data, curr, dataSize);

	free_buffer(cp_buff);
	free_buffer(buff);

	return data;
}

void TextureSerializer::free_buffer(unsigned char* data)
{
	free(data);
}