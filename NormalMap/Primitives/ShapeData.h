#pragma once
#include "Vertex.h"
#include "TypeDefs.h"

template<class TVert>
struct ShapeDataTemplate
{
	TVert* verts;
	uint numVerts;
	ushort* indices;
	uint numIndices;
	char* textureFileName;
	char* normalMapFileName;
	char* underlyingBuffer;

	inline ShapeDataTemplate();
	inline uint vertexBufferSize() const;
	inline uint indexBufferSize() const;
	inline uint totalBufferSize() const;
	inline void cleanUp();
	inline bool isValid();
};

typedef ShapeDataTemplate<VertexPCNUT> ShapeData;
typedef ShapeDataTemplate<VertexPNUT> ShapeDataPnut;
typedef ShapeDataTemplate<VertexPC> ShapeDataPc;


template<class TVert>
ShapeDataTemplate<TVert>::ShapeDataTemplate() :
verts(0), numVerts(0), indices(0), numIndices(0),
textureFileName(0), normalMapFileName(0), underlyingBuffer(0) {}

template<class TVert>
uint ShapeDataTemplate<TVert>::vertexBufferSize() const { return numVerts * sizeof(TVert); }

template<class TVert>
uint ShapeDataTemplate<TVert>::indexBufferSize() const { return numIndices * sizeof(ushort); }

template<class TVert>
uint ShapeDataTemplate<TVert>::totalBufferSize() const
{
	return vertexBufferSize() + indexBufferSize();
}

template<class TVert>
bool ShapeDataTemplate<TVert>::isValid()
{
	return underlyingBuffer != 0 || (verts != 0 && indices != 0);
}

template<class TVert>
void ShapeDataTemplate<TVert>::cleanUp()
{
	if (underlyingBuffer != 0)
	{
		delete[] underlyingBuffer;
	}
	else
	{
		delete[] verts;
		delete[] indices;
	}
	if (textureFileName != NULL)
	{
		delete[] textureFileName;
		textureFileName = 0;
	}
	numVerts = numIndices = 0;
	verts = 0;
	indices = 0;
}




/*#pragma once
#include <GL\glew.h>
#include <Primitives\Vertex.h>

typedef unsigned int uint;
typedef unsigned short ushort;

struct ShapeData
{
	ShapeData() :
		vertices(0), numVertices(0),
		indices(0), numIndices(0){}

	Vertex* vertices;
	uint numVertices;
	ushort* indices;
	uint numIndices;

	GLsizeiptr vertexBufferSize() const
	{
		return numVertices * sizeof(Vertex);
	}
	GLsizeiptr indexBufferSize() const
	{
		return numIndices * sizeof(ushort);
	}
	void cleanUp()
	{
		delete[] vertices;
		delete[] indices;
		numVertices = numIndices = 0;
	}
};*/