#include "PhotonBox/resource/Mesh.h"

#include "PhotonBox/util/OBJLoader.h"
#include "PhotonBox/util/FileWatch.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

Mesh::Mesh(Config config)
	: _config(config)
{
	_isLoaded = true;
	_isInitialized = false;
	blankInitialize();
}

Mesh::Mesh(Filepath filePath, Config config)
	: _config(config)
{
	FileWatch::addToWatchList(filePath.getAbsolutePath(), this);
	_filePath = filePath;

	if (!config.forceInit)
	{
		std::cout << "Index Mesh: " << filePath.getAbsolutePath() << std::endl;
		loadAsync();
	}
	else
	{
		forceLoad();
	}
}

GLuint Mesh::getVAO()
{
	return _vao;
}

GLuint Mesh::getEBO()
{
	return _ebo;
}

void Mesh::loadFromFile() 
{ 
	OBJLoader::loadObj(_filePath.getAbsolutePath(), this);
}

void Mesh::blankInitialize()
{
	vertices = {Vertex(Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f::ZERO)};
	indices = {0};
	boundingSphere = BoundingSphere(Vector3f::ZERO, 1);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &(vertices[0]), GL_STATIC_DRAW);

	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(Vertex::AttibLocation::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(Vertex::AttibLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(Vertex::AttibLocation::COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(Vertex::AttibLocation::TEXTURECOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glVertexAttribPointer(Vertex::AttibLocation::TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	vertices.clear();
	indices.clear();
}


void Mesh::submitBuffer() 
{
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &(vertices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//std::cout << "Initialized: " << _fileName << "\n";
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao);
}