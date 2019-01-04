#include "PhotonBox/core/MeshSerializer.h"

#include <fstream>
#include <iostream>

#include "PhotonBox/resources/Mesh.h"
#include "PhotonBox/resources/Vertex.h"
#include "PhotonBox/core/Util.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

const short MeshSerializer::SERIALIZER_VERSION = 1;
const std::string MeshSerializer::EXTENSION = ".pbm";

void MeshSerializer::write(const std::string &pathName, Mesh* mesh)
{
	std::fstream f = std::fstream(pathName, std::ios::out | std::ios::binary);
	if (f.is_open())
	{
		int vert_cnt = static_cast<int>(mesh->vertices.size());
		int ind_cnt = static_cast<int>(mesh->indices.size());
		BoundingSphere bs = mesh->boundingSphere;

		size_t vert_size = sizeof(Vertex) * vert_cnt;
		size_t ind_size = sizeof(unsigned int) * ind_cnt;
		size_t buff_size = sizeof(int) + sizeof(int) + sizeof(BoundingSphere) + vert_size + ind_size;
		size_t cp_buff_size = 0;

		// Create one buffer from all the components
		unsigned char* buff = Util::combineBuffer(10,
			(unsigned char*)&vert_cnt, sizeof(int),
			(unsigned char*)&ind_cnt, sizeof(int),
			(unsigned char*)&bs, sizeof(BoundingSphere),
			&(mesh->vertices[0]), vert_size,
			&(mesh->indices[0]), ind_size
		);

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
		std::cout << "ERROR: Could not create " << pathName << std::endl;
	}
}

void MeshSerializer::read(const std::string & pathName, Mesh* mesh)
{

	std::fstream f = std::fstream(pathName, std::ios::in | std::ios::binary);
	if (f.is_open())
	{
		size_t buff_size = -1;
		size_t cp_buff_size = -1;
		short version = -1;
		unsigned char* cp_buff;

		int vert_cnt = -1;
		int ind_cnt = -1;

		size_t vert_size = -1;
		size_t ind_size = -1;

		f.seekg(0);
		f.read((char*)&version, sizeof(short));
		if (version != SERIALIZER_VERSION)
			std::cerr << "ERROR: Old or unsupported Texture version!\n";

		f.read((char*)&buff_size, sizeof(size_t));
		f.read((char*)&cp_buff_size, sizeof(size_t));

		cp_buff = (unsigned char*)malloc(cp_buff_size);

		f.read((char*)cp_buff, cp_buff_size);
		f.close();

		unsigned char* buff = Util::decompress(cp_buff, cp_buff_size, buff_size);
		unsigned char* curr = buff;

		vert_cnt = *reinterpret_cast<int*>(curr);
		curr += sizeof(int);
		ind_cnt = *reinterpret_cast<int*>(curr);
		curr += sizeof(int);
		mesh->boundingSphere = *reinterpret_cast<BoundingSphere*>(curr);
		curr += sizeof(BoundingSphere);

		if (vert_cnt < 0 || ind_cnt < 0)
		{
			// Looks like thexture was incorrectly inflated
			__debugbreak();
		}

		vert_size = sizeof(Vertex) * vert_cnt;
		ind_size = sizeof(unsigned int) * ind_cnt;
		
		mesh->vertices.resize(vert_cnt);
		mesh->indices.resize(ind_cnt);

		memcpy(&(mesh->vertices[0]), curr, vert_size);
		curr += vert_size;
		memcpy(&(mesh->indices[0]), curr, ind_size);

		free_buffer(cp_buff);
		free_buffer(buff);
	}
	else
	{
		std::cout << "ERROR: Could not read " << pathName << std::endl;
	}
}

void MeshSerializer::free_buffer(unsigned char* data)
{
	free(data);
}
