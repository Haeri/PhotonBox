#include <iostream>
#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <PhotonBox/core/TextureSerializer.h>



#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

int main(int argc, char* argv[])
{

	for (int i = 1; i < argc; ++i)
	{
		std::cout << argv[i] << std::endl;

		unsigned char* _data = NULL;
		int numComponents;
		int width;
		int height;

		std::string filePath = argv[i];

		_data = stbi_load((filePath).c_str(), &width, &height, &numComponents, STBI_rgb_alpha);
		TextureSerializer::write(filePath + TextureSerializer::EXTENSION, width, height, 4, _data);
	}

#ifdef _DEBUG
	std::cin.ignore();
#endif

	return 0;
}