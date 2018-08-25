#include "PhotonBox/core/MeshSerializer.h"

#include <fstream>
#include <iostream>

#include "PhotonBox/resources/Mesh.h"
#include "PhotonBox/resources/Vertex.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void MeshSerializer::write(const std::string & pathName, Mesh* mesh)
{

	int indicesSize = static_cast<int>(mesh->indices.size());
	int verticesSize = static_cast<int>(mesh->vertices.size());
	BoundingSphere bs = mesh->boundingSphere;

	std::ofstream myfile;
	myfile.open(pathName, std::ios::out | std::ios::binary);

	if (myfile.is_open())
	{
		myfile.seekp(0);
		// write vertices size
		myfile.write((char*)&verticesSize, sizeof(int));
		// write indices size
		myfile.write((char*)&indicesSize, sizeof(int));
		// write Bounding Sphere
		myfile.write((char*)&bs, sizeof(BoundingSphere));
		// write indices
		myfile.write((char*)&mesh->vertices[0], sizeof(Vertex) * verticesSize);
		// write vertices
		myfile.write((char*)&mesh->indices[0], sizeof(unsigned int) * indicesSize);

		myfile.close();
	}
	else
	{
		std::cout << "Unable to write to file" << std::endl;
	}
}

void MeshSerializer::read(const std::string & pathName, Mesh* mesh)
{
	int indicesSize;
	int verticesSize; 
	BoundingSphere bs;


	std::ifstream myfile;
	myfile.open(pathName, std::ios::in | std::ios::binary);

	if (myfile.is_open())
	{
		myfile.seekg(0);

		// read vertices size
		myfile.read((char*)&verticesSize, sizeof(int));
		// read indices size
		myfile.read((char*)&indicesSize, sizeof(int));
		// read Bounding Sphere
		myfile.read((char*)&bs, sizeof(BoundingSphere));

		mesh->vertices.resize(verticesSize);
		mesh->indices.resize(indicesSize);

		// read indices
		myfile.read((char*)&(mesh->vertices[0]), sizeof(Vertex) * verticesSize);
		// read vertices
		myfile.read((char*)&(mesh->indices[0]), sizeof(unsigned int) * indicesSize);

		myfile.close();

		mesh->boundingSphere = bs;
	}
	else
	{
		std::cout << "Unable to open file" << std::endl;
	}
}
