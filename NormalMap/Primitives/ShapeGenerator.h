#ifndef SHAPE_GENERATOR_H
#define SHAPE_GENERATOR_H
#include <glm\glm.hpp>
#include "ShapeData.h"

// After you call any function in this class, you are responsible to delete
// all the data referenced by the returned ShapeData. You can call
// ShapeData::cleanUp() to do this for you if you like.
class ShapeGenerator
{
public:
	// Makes a line from (-1, 0, 0) to (1, 0, 0)
	// Must render with GL_LINES
	static ShapeData makeLine();
	static ShapeData make2DTriangle();
	static ShapeData makeHudQuad();
	static ShapeData makeCone(uint tesselation = 10);
	static ShapeData makeCylinder(uint tesselation = 10);
	// Render GL_TRIANGLES
	static ShapeData makePlane(uint dimensions = 1);
	// Render GL_LINES
	static ShapeData makeWireframePlane(uint dimensions = 1);
	static ShapeData makeCube();
	static ShapeData makeSphere(uint tesselation = 10);
	static ShapeData makeTorus(uint tesselation = 10);
	static ShapeData makeArrow();
	static ShapeData makeTeapot(uint tesselation = 10, const glm::mat4& lidTransform = glm::mat4());

	// Generates a ShapeData for GL_LINES that will render 
	// white normals for the given ShapeData. Only the Vertex 
	// positions and colors will have any meaning
	template<class TVert>
	static ShapeDataTemplate<VertexPC> generateNormals(const ShapeDataTemplate<TVert>& data);
	template<class TVert>
	static ShapeDataPc generateNormalsAndTangents(const ShapeDataTemplate<TVert>& data);
	static const float CONE_HEIGHT;
private:
	static ShapeData copyToShapeData(
		VertexPCNUT* verts, size_t numVerts,
		ushort* indices, uint numIndices);
	static void makeTeapot(
		unsigned int grid, const glm::mat4& lidTransform,
		unsigned int& numVerts,
		float*& verts,
		float*& normals,
		float*& textureCoordinates,
		unsigned short*& indices,
		unsigned int& numIndices);

	static ShapeData makePlaneVerts(uint tesselation);
	static ShapeData makePlaneIndices(uint tesselation);
	static ShapeData makePlaneUnseamedIndices(uint tesselation);

	// Teapot:
	static void generatePatches(float * v, float * n, float *tc, unsigned short* el, int grid);
	static void buildPatchReflect(int patchNum,
		float *B, float *dB,
		float *v, float *n, float *, unsigned short *el,
		int &index, int &elIndex, int &, int grid,
		bool reflectX, bool reflectY);
	static void buildPatch(glm::vec3 patch[][4],
		float *B, float *dB,
		float *v, float *n, float *, unsigned short *el,
		int &index, int &elIndex, int &, int grid, glm::mat3 reflect, bool invertNormal);
	static void getPatch(int patchNum, glm::vec3 patch[][4], bool reverseV);
	static void computeBasisFunctions(float * B, float * dB, int grid);
	static glm::vec3 evaluate(int gridU, int gridV, float *B, glm::vec3 patch[][4]);
	static glm::vec3 evaluateNormal(int gridU, int gridV, float *B, float *dB, glm::vec3 patch[][4]);
	static void moveLid(int, float *, glm::mat4);
};

template<class TVert>
static ShapeDataPc ShapeGenerator::generateNormals(const ShapeDataTemplate<TVert>& data)
{
	ShapeDataPc ret;
	ret.numVerts = data.numVerts * 2;
	ret.verts = new VertexPC[ret.numVerts];
	glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < data.numVerts; i++)
	{
		uint vertIndex = i * 2;
		VertexPC& v1 = ret.verts[vertIndex];
		VertexPC& v2 = ret.verts[vertIndex + 1];
		const TVert& sourceVertex = data.verts[i];
		v1.position = sourceVertex.position;
		v2.position = sourceVertex.position + 0.05f * sourceVertex.normal;
		v1.color = v2.color = white;
	}

	ret.numIndices = ret.numVerts;
	ret.indices = new ushort[ret.numIndices];
	for (int i = 0; i < ret.numIndices; i++)
		ret.indices[i] = i;
	return ret;
}

template<class TVert>
static ShapeDataPc ShapeGenerator::generateNormalsAndTangents(const ShapeDataTemplate<TVert>& data)
{
	ShapeDataPc ret;
	ret.numVerts = data.numVerts * 4;
	ret.verts = new VertexPC[ret.numVerts];
	glm::vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
	for (int i = 0; i < data.numVerts; i++)
	{
		uint vertIndex = i * 4;
		VertexPC& v1 = ret.verts[vertIndex + 0];
		VertexPC& v2 = ret.verts[vertIndex + 1];
		VertexPC& t1 = ret.verts[vertIndex + 2];
		VertexPC& t2 = ret.verts[vertIndex + 3];
		const TVert& sourceVertex = data.verts[i];
		float scale = 0.05f;

		v1.position = sourceVertex.position;
		v2.position = sourceVertex.position + scale * sourceVertex.normal;
		v1.color = v2.color = red;

		t1.position = sourceVertex.position;
		t2.position = sourceVertex.position + scale * sourceVertex.tangent;
		t1.color = t2.color = green;
	}

	ret.numIndices = ret.numVerts;
	ret.indices = new ushort[ret.numIndices];
	for (int i = 0; i < ret.numIndices; i++)
		ret.indices[i] = i;
	return ret;
}

#endif


/*#pragma once
#include <glm\glm.hpp>
#include <Primitives\ShapeData.h>

class ShapeGenerator
{
	static ShapeData makePlaneUnseamedIndices(GLuint tesselation);

public:
	static ShapeData makePlaneVerts(uint dimensions);
	static ShapeData makePlaneIndices(uint dimensions);
	static ShapeData makeCube();
	static ShapeData makePlane(uint dimensions);
	static ShapeData generateNormals(const ShapeData& data);
	static ShapeData makeSphere(GLuint tesselation);
	static ShapeData makeTorus(GLuint tesselation);
};*/