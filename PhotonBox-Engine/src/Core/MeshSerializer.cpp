#include <fstream>
#include <iostream>

#include "../Resources/Vertex.h"
#include "MeshSerializer.h"
#include "../Resources/Mesh.h"

void MeshSerializer::write(const std::string & pathName, Mesh* mesh){
	
	int indicesSize = mesh->indices.size();
	int verticesSize = mesh->vertices.size();
	float boundingSphereRadius = mesh->boundingSphereRadius;

	std::ofstream myfile;
	myfile.open(pathName, std::ios::out | std::ios::binary);

	if (myfile.is_open()) {
		myfile.seekp(0);
		// write vertices size
		myfile.write((char*)&verticesSize, sizeof(int));
		// write indices size
		myfile.write((char*)&indicesSize, sizeof(int));
		// write radius
		myfile.write((char*)&boundingSphereRadius, sizeof(float));
		// write indices
		myfile.write((char*)&mesh->vertices[0], sizeof(Vertex) * verticesSize);
		// write vertices
		myfile.write((char*)&mesh->indices[0], sizeof(unsigned int) * indicesSize);

		myfile.close();
	}else {
		std::cout << "Unable to write to file" << std::endl;
	}
}

Mesh * MeshSerializer::read(const std::string & pathName){
	Mesh* mesh;

	int indicesSize;
	int verticesSize;
	float boundingSphereRadius;

	Vertex *vertices;
	unsigned int *indices;

	std::ifstream myfile;
	myfile.open(pathName, std::ios::in | std::ios::binary);

	if (myfile.is_open()) {
		myfile.seekg(0);

		// read vertices size
		myfile.read((char*)&verticesSize, sizeof(int));
		// read indices size
		myfile.read((char*)&indicesSize, sizeof(int));
		// read radius
		myfile.read((char*)&boundingSphereRadius, sizeof(float));

		vertices = new Vertex[verticesSize];
		indices = new unsigned int[indicesSize];

		// read indices
		myfile.read((char*)&vertices[0], sizeof(Vertex) * verticesSize);
		// read vertices
		myfile.read((char*)&indices[0], sizeof(unsigned int) * indicesSize);

		myfile.close();


		mesh = new Mesh();
		mesh->boundingSphereRadius = boundingSphereRadius;
		mesh->vertices.assign(vertices, vertices + verticesSize);
		mesh->indices.assign(indices, indices + indicesSize);
		return mesh;
	}else {
		std::cout << "Unable to open file" << std::endl;
		return nullptr;
	}
}
