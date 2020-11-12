#ifndef MESH_SERIALIZER_H
#define MESH_SERIALIZER_H

class Mesh;

#include <string>

/*
*	Extension: .pbm
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
*	vertex_cnt		int				4				Amount of vertices in the vertices data
*	index_cnt		int				4				Amount of indices in the indices data
*	bs				BoundingSphere	16				Boundingsphere data
*	vertices		Vertex*			{}				Vertices
*	indices			unsigned ind*	{}				Indices
*
*/

class MeshSerializer
{
public:
	static const short SERIALIZER_VERSION;
	static const std::string EXTENSION;

	static void write(const std::string& pathName, Mesh* mesh);
	static void read(const std::string & pathName, Mesh* mesh);
	static void free_buffer(unsigned char* data);
};

#endif // MESH_SERIALIZER_H
