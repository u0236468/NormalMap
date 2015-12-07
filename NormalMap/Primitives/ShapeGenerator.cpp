#include "ShapeGenerator.h"
#include <glm\gtx\transform.hpp>
#include "Vertex.h"
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

const float ShapeGenerator::CONE_HEIGHT = 0.35f;
#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

glm::vec4 randomColor()
{
	glm::vec4 ret;
	ret.x = rand() / (float)RAND_MAX;
	ret.y = rand() / (float)RAND_MAX;
	ret.z = rand() / (float)RAND_MAX;
	ret.w = 1;
	return ret;
}

ShapeData ShapeGenerator::make2DTriangle()
{
	VertexPCNUT stackVerts[] =
	{
		vec3(+0.0f, +1.0f, +0.0f),			// position
		vec4(+1.0f, +0.0f, +0.0f, +0.0f),	// color
		vec3(+0.0f, +0.0f, +1.0f),			// normal
		vec2(+0.5f, +0.0f),					// uv
		vec3(+1.0f, +0.0f, 0.0f),			// tangent

		vec3(-1.0f, -1.0f, +0.0f),			// position
		vec4(+0.0f, +1.0f, +0.0f, +0.0f),	// color
		vec3(+0.0f, +0.0f, +1.0f),			// normal
		vec2(+0.0f, +1.0f),					// uv
		vec3(+1.0f, +0.0f, 0.0f),			// tangent

		vec3(+1.0f, -1.0f, +0.0f),			// position
		vec4(+0.0f, +0.0f, +1.0f, +0.0f),	// color
		vec3(+0.0f, +0.0f, +1.0f),			// normal
		vec2(+1.0f, +1.0f),					// uv
		vec3(+1.0f, +0.0f, 0.0f),			// tangent
	};
	unsigned short stackIndices[] = { 0, 1, 2 };
	return copyToShapeData(stackVerts, ARRAY_SIZE(stackVerts), stackIndices, ARRAY_SIZE(stackIndices));
}

ShapeData ShapeGenerator::makeHudQuad()
{
	float closestZ = 0.999f;
	VertexPCNUT stackVerts[] =
	{
		vec3(-1.0f, +1.0f, closestZ),			// position
		vec4(+1.0f, +1.0f, +1.0f, +0.0f),	// color
		vec3(+0.0f, +0.0f, +1.0f),			// normal
		vec2(+0.0f, +1.0f),					// uv
		vec3(+1.0f, +0.0f, 0.0f),			// tangent

		vec3(+1.0f, +1.0f, closestZ),			// position
		vec4(+1.0f, +1.0f, +1.0f, +0.0f),	// color
		vec3(+0.0f, +0.0f, +1.0f),			// normal
		vec2(+1.0f, +1.0f),					// uv
		vec3(+1.0f, +0.0f, 0.0f),			// tangent

		vec3(-1.0f, -1.0f, closestZ),			// position
		vec4(+1.0f, +1.0f, +1.0f, +0.0f),	// color
		vec3(+0.0f, +0.0f, +1.0f),			// normal
		vec2(+0.0f, +0.0f),					// uv
		vec3(+1.0f, +0.0f, 0.0f),			// tangent

		vec3(+1.0f, -1.0f, closestZ),			// position
		vec4(+1.0f, +1.0f, +1.0f, +0.0f),	// color
		vec3(+0.0f, +0.0f, +1.0f),			// normal
		vec2(+1.0f, +0.0f),					// uv
		vec3(+1.0f, +0.0f, 0.0f),			// tangent
	};
	unsigned short stackIndices[] = { 0, 1, 3, 0, 2, 3 };
	return copyToShapeData(stackVerts, ARRAY_SIZE(stackVerts), stackIndices, ARRAY_SIZE(stackIndices));
}

ShapeData ShapeGenerator::makeLine()
{
	ShapeData ret;
	glm::vec4 white(1, 1, 1, 1);

	ret.numVerts = 2;
	ret.verts = new VertexPCNUT[ret.numVerts];

	VertexPCNUT& first = ret.verts[0];
	first.position = vec3(0, 0, 0);
	first.color = white;
	VertexPCNUT& second = ret.verts[1];
	second.position = vec3(1, 0, 0);
	second.color = white;

	ret.numIndices = 2;
	ret.indices = new ushort[ret.numIndices];
	ret.indices[0] = 0;
	ret.indices[1] = 1;

	return ret;
}

ShapeData ShapeGenerator::makeCone(uint tesselation)
{
	// Cone head
	vec3 coneTip(CONE_HEIGHT, 0.0f, 0.0f);
	float coneRadius = 0.2f;

	// Rotate thisVert around x axis
	vec4 thisVert(0, coneRadius, 0, 0);
	glm::mat4 rotator = glm::rotate(360.0f / tesselation, glm::vec3(1.0f, 0.0f, 0.0f));

	ShapeData ret;
	// 2 verts at same position around the rim so I can vary the normals
	// + 2 -> one for cone tip and one for cone base center
	ret.numVerts = 2 * tesselation + 2;
	ret.verts = new VertexPCNUT[ret.numVerts];

	uint coneTipIndex = tesselation * 2;
	uint coneBaseCenterIndex = coneTipIndex + 1;
	for (uint i = 0; i < tesselation; i++)
	{
		// Side facing triangle vert
		VertexPCNUT& v = ret.verts[i];
		v.color = randomColor();
		v.position = vec3(thisVert);
		// Just want its position in the YZ plane, ignore x value;
		v.normal = glm::normalize(glm::vec3(0, v.position.y, v.position.z));
		thisVert = rotator * thisVert;
	}

	for (uint i = tesselation; i < tesselation * 2; i++)
	{
		// Cone bottom facing vert. Everything is same except the normal
		VertexPCNUT& v = ret.verts[i - tesselation];
		VertexPCNUT& v2 = ret.verts[i];
		v2.color = v.color;
		v2.position = v.position;
		v2.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	}

	ret.verts[coneTipIndex].position = coneTip;
	ret.verts[coneTipIndex].color = randomColor();
	ret.verts[coneTipIndex].normal = glm::vec3(1.0f, 0.0f, 0.0f);

	ret.verts[coneBaseCenterIndex].position = glm::vec3(0, 0.0f, 0.0f);
	ret.verts[coneBaseCenterIndex].color = randomColor();
	ret.verts[coneBaseCenterIndex].normal = glm::vec3(-1.0f, 0.0f, 0.0f);


	const uint NUM_VERTS_PER_TRI = 3;
	ret.numIndices = NUM_VERTS_PER_TRI * tesselation * 2;
	ret.indices = new ushort[ret.numIndices];

	uint indiceIndex = 0;
	for (uint i = 0; i < tesselation; i++)
	{
		// Side face
		ret.indices[indiceIndex++] = i;
		ret.indices[indiceIndex++] = (i + 1) % tesselation;
		ret.indices[indiceIndex++] = coneTipIndex;
	}
	for (uint i = tesselation; i < tesselation * 2; i++)
	{
		// Bottom face
		ret.indices[indiceIndex++] = i;
		ret.indices[indiceIndex++] = (i + 1) % tesselation;
		ret.indices[indiceIndex++] = coneBaseCenterIndex;
	}
	assert(indiceIndex == ret.numIndices);

	// Cynlindar stem
	return ret;
}

ShapeData ShapeGenerator::makeCylinder(uint tesselation)
{
	ShapeData ret;
	ret.numVerts = tesselation * 2 + 2; // + 2 for top and bottom center
	ret.verts = new VertexPCNUT[ret.numVerts];

	vec4 thisVert(0, 1, 0, 0);
	glm::mat4 rotator = glm::rotate(360.0f / tesselation, glm::vec3(1.0f, 0.0f, 0.0f));

	for (uint i = 0; i < tesselation; i++)
	{
		VertexPCNUT& v0 = ret.verts[i];
		VertexPCNUT& v1 = ret.verts[i + tesselation];

		v0.position = vec3(thisVert);
		v0.position.x = 1.0f;
		v1.position = vec3(thisVert);

		v0.color = randomColor();
		v1.color = randomColor();
		v0.normal.x = +1.0f;
		v1.normal.x = -1.0f;

		thisVert = rotator * thisVert;
	}
	uint topCenterVertIndex = ret.numVerts - 2;
	uint bottomCenterVertIndex = ret.numVerts - 1;
	ret.verts[topCenterVertIndex].position.x = 1.0f;
	ret.verts[topCenterVertIndex].normal.x = +1.0f;
	ret.verts[topCenterVertIndex].color = randomColor();
	ret.verts[bottomCenterVertIndex].normal.x = -1.0f;
	ret.verts[bottomCenterVertIndex].color = randomColor();

	const uint NUM_TRIS_PER_SIDE_FACE = 2;
	const uint NUM_TRIS_PER_END_FACE = 1;
	const uint NUM_ENDS = 2;
	const uint NUM_VERTS_PER_TRI = 3;
	const uint NUM_SIDE_INDICES = tesselation * NUM_TRIS_PER_SIDE_FACE * NUM_VERTS_PER_TRI;
	const uint NUM_END_INDICES = tesselation * NUM_TRIS_PER_END_FACE * NUM_VERTS_PER_TRI * NUM_ENDS;
	ret.numIndices = NUM_SIDE_INDICES + NUM_END_INDICES;
	ret.indices = new ushort[ret.numIndices];

	uint indiceIndex = 0;
	for (uint i = 0; i < tesselation; i++)
	{
		// Top
		ret.indices[indiceIndex++] = i;
		ret.indices[indiceIndex++] = (i + 1) % tesselation;
		ret.indices[indiceIndex++] = topCenterVertIndex;
	}
	for (uint i = 0; i < tesselation; i++)
	{
		// Bottom
		ret.indices[indiceIndex++] = i + tesselation;
		ret.indices[indiceIndex++] = (i + 1 < tesselation) ? i + 1 + tesselation : tesselation;
		ret.indices[indiceIndex++] = bottomCenterVertIndex;
	}

	// Side
	for (uint i = 0; i < tesselation; i++)
	{
		// Face 1
		ret.indices[indiceIndex++] = i;
		ret.indices[indiceIndex++] = (i + 1) % tesselation;
		ret.indices[indiceIndex++] = i + tesselation;

		// Face 2
		ret.indices[indiceIndex++] = (i + 1) % tesselation;
		ret.indices[indiceIndex++] = (i + 1 < tesselation) ? (i + 1 + tesselation) : i + 1;
		ret.indices[indiceIndex++] = i + tesselation;
	}
	assert(indiceIndex == ret.numIndices);
	return ret;
}

ShapeData ShapeGenerator::makePlaneVerts(uint dimensions)
{
	ShapeData ret;
	uint vertsPerDimension = dimensions + 1; // + 1 for last row/col of verts
	ret.numVerts = vertsPerDimension * vertsPerDimension;
	ret.verts = new VertexPCNUT[ret.numVerts];
	float half = dimensions / 2.0f;
	glm::vec3 runner(-half, 0.0f, -half);
	for (int i = 0; i < vertsPerDimension; i++)
	{
		for (int j = 0; j < vertsPerDimension; j++)
		{
			VertexPCNUT& thisVert = ret.verts[i * vertsPerDimension + j];
			thisVert.position = runner;
			thisVert.color = randomColor();
			thisVert.normal = glm::vec3(0, 1, 0);
			thisVert.tangent = glm::vec3(1, 0, 0);
			thisVert.uv.x = j / (float)dimensions;
			thisVert.uv.y = i / (float)dimensions;
			runner.x++;
		}
		runner.x = -half;
		runner.z++;
	}
	return ret;
}

ShapeData ShapeGenerator::makePlaneIndices(uint dimensions)
{
	ShapeData ret;
	uint vertsPerDimension = dimensions + 1;
	ret.numIndices = dimensions * dimensions * 2 * 3; // 2 triangles per square, 3 indices per triangle
	ret.indices = new unsigned short[ret.numIndices];
	int runner = 0;
	for (int row = 0; row < dimensions; row++)
	{
		for (int col = 0; col < dimensions; col++)
		{
			ret.indices[runner++] = vertsPerDimension * row + col;
			ret.indices[runner++] = vertsPerDimension * row + col + vertsPerDimension;
			ret.indices[runner++] = vertsPerDimension * row + col + vertsPerDimension + 1;

			ret.indices[runner++] = vertsPerDimension * row + col;
			ret.indices[runner++] = vertsPerDimension * row + col + vertsPerDimension + 1;
			ret.indices[runner++] = vertsPerDimension * row + col + 1;
		}
	}
	assert(runner = ret.numIndices);
	return ret;
}

ShapeData ShapeGenerator::makePlaneUnseamedIndices(uint tesselation)
{
	ShapeData ret;
	uint dimensions = tesselation * tesselation;
	ret.numIndices = dimensions * 2 * 3; // 2 triangles per square, 3 indices per triangle
	ret.indices = new unsigned short[ret.numIndices];
	int runner = 0;
	for (int row = 0; row < tesselation; row++)
	{
		// This code is crap but works, and I'm not in the mood right now to clean it up
		for (int col = 0; col < tesselation; col++)
		{
			// Bottom left triangle
			ret.indices[runner++] = tesselation * row + col;
			// One row down unless it's the bottom row, 
			ret.indices[runner++] = (row + 1 == tesselation ? 0 : tesselation * row + tesselation) + col;
			// Move to vert right of this one unless it's the last vert,
			// which we connect to the first vert in the row
			// the % dimensions at the end accounts for the last row hooking to the first row
			ret.indices[runner++] = (tesselation * row + col + tesselation + (col + 1 == tesselation ? -tesselation + 1 : 1)) % dimensions;

			// Upper right triangle
			ret.indices[runner++] = tesselation * row + col;
			if (col + 1 == tesselation && row + 1 == tesselation)
			{
				// Very last vert
				ret.indices[runner++] = 0;
			}
			else if (col + 1 == tesselation)
			{
				// Last vert on this row
				// Check if we need to connect it to zeroeth row or the next row
				if (row + 1 == tesselation)
				{
					// Tie to zeroeth row
					ret.indices[runner++] = col + 1;
				}
				else
				{
					// Tie to next row
					ret.indices[runner++] = tesselation * row + col + 1;
				}
			}
			else
			{
				// Regular interior vert
				// the % dimensions at the end accounts for the last row hooking to the first row
				ret.indices[runner++] = (tesselation * row + col + tesselation + 1) % dimensions;
			}
			ret.indices[runner++] = tesselation * row + col + (col + 1 == tesselation ? -col : 1);
		}
	}
	return ret;
}

ShapeData ShapeGenerator::makePlane(uint dimensions)
{
	ShapeData ret = makePlaneVerts(dimensions);
	ShapeData ret2 = makePlaneIndices(dimensions);
	ret.numIndices = ret2.numIndices;
	ret.indices = ret2.indices;
	return ret;
}

ShapeData ShapeGenerator::makeWireframePlane(uint dimensions)
{
	ShapeData ret;
	if (dimensions % 2 == 0)
		dimensions++;
	ret.numVerts = dimensions * 2 * 2; // 2 verts per line, 2 dimensions
	const int HALF = dimensions / 2;
	ret.verts = new VertexPCNUT[ret.numVerts];
	int runner = 0;
	glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < ret.numVerts; i += 4, runner++)
	{
		// Set up criss cross line endpoints
		VertexPCNUT& v1 = ret.verts[i + 0];
		v1.position.x = -HALF;
		v1.position.y = 0;
		v1.position.z = HALF + -runner;
		v1.color = white;

		VertexPCNUT& v2 = ret.verts[i + 1];
		v2.position.x = HALF;
		v2.position.y = 0;
		v2.position.z = HALF + -runner;
		v2.color = white;

		VertexPCNUT& v3 = ret.verts[i + 2];
		v3.position.x = -HALF + runner;
		v3.position.y = 0;
		v3.position.z = HALF;
		v3.color = white;

		VertexPCNUT& v4 = ret.verts[i + 3];
		v4.position.x = -HALF + runner;
		v4.position.y = 0;
		v4.position.z = -HALF;
		v4.color = white;
	}
	ret.numIndices = (dimensions + dimensions) * 2;
	ret.indices = new ushort[ret.numIndices];
	for (uint i = 0; i < ret.numIndices; i++)
		ret.indices[i] = i;
	return ret;
}

ShapeData ShapeGenerator::makeCube()
{
	using glm::vec2;
	using glm::vec3;
	using glm::vec4;
	VertexPCNUT stackVerts[] =
	{
		// Top
		vec3(-1.0f, +1.0f, +1.0f),        // 0
		vec4(+1.0f, +0.0f, +0.0f, +1.0f), // Color
		vec3(+0.0f, +1.0f, +0.0f),        // Normal
		vec2(+0.0f, +1.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent
		vec3(+1.0f, +1.0f, +1.0f),        // 1
		vec4(+0.0f, +1.0f, +0.0f, +1.0f), // Color
		vec3(+0.0f, +1.0f, +0.0f),        // Normal
		vec2(+1.0f, +1.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent
		vec3(+1.0f, +1.0f, -1.0f),        // 2
		vec4(+0.0f, +0.0f, +1.0f, +1.0f), // Color
		vec3(+0.0f, +1.0f, +0.0f),        // Normal
		vec2(+1.0f, +0.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent
		vec3(-1.0f, +1.0f, -1.0f),        // 3
		vec4(+1.0f, +1.0f, +1.0f, +1.0f), // Color
		vec3(+0.0f, +1.0f, +0.0f),        // Normal
		vec2(+0.0f, +0.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent

		// Front
		vec3(-1.0f, +1.0f, -1.0f),        // 4
		vec4(+1.0f, +0.0f, +1.0f, +1.0f), // Color
		vec3(+0.0f, +0.0f, -1.0f),        // Normal
		vec2(+0.0f, +1.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent
		vec3(+1.0f, +1.0f, -1.0f),        // 5
		vec4(+0.0f, +0.5f, +0.2f, +1.0f), // Color
		vec3(+0.0f, +0.0f, -1.0f),        // Normal
		vec2(+1.0f, +1.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent
		vec3(+1.0f, -1.0f, -1.0f),        // 6
		vec4(+0.8f, +0.6f, +0.4f, +1.0f), // Color
		vec3(+0.0f, +0.0f, -1.0f),        // Normal
		vec2(+1.0f, +0.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent
		vec3(-1.0f, -1.0f, -1.0f),        // 7
		vec4(+0.3f, +1.0f, +0.5f, +1.0f), // Color
		vec3(+0.0f, +0.0f, -1.0f),        // Normal
		vec2(+0.0f, +0.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent

		// Right
		vec3(+1.0f, +1.0f, -1.0f),        // 8
		vec4(+0.2f, +0.5f, +0.2f, +1.0f), // Color
		vec3(+1.0f, +0.0f, +0.0f),        // Normal
		vec2(+1.0f, +0.0f),               // UV
		vec3(+0.0f, +0.0f, -1.0f),		  // Tangent
		vec3(+1.0f, +1.0f, +1.0f),        // 9
		vec4(+0.9f, +0.3f, +0.7f, +1.0f), // Color
		vec3(+1.0f, +0.0f, +0.0f),        // Normal
		vec2(+0.0f, +0.0f),               // UV
		vec3(+0.0f, +0.0f, -1.0f),		  // Tangent
		vec3(+1.0f, -1.0f, +1.0f),        // 10
		vec4(+0.3f, +0.7f, +0.5f, +1.0f), // Color
		vec3(+1.0f, +0.0f, +0.0f),        // Normal
		vec2(+0.0f, +1.0f),               // UV
		vec3(+0.0f, +0.0f, -1.0f),		  // Tangent
		vec3(+1.0f, -1.0f, -1.0f),        // 11
		vec4(+0.5f, +0.7f, +0.5f, +1.0f), // Color
		vec3(+1.0f, +0.0f, +0.0f),        // Normal
		vec2(+1.0f, +1.0f),               // UV
		vec3(+0.0f, +0.0f, -1.0f),		  // Tangent

		// Left
		vec3(-1.0f, +1.0f, +1.0f),        // 12
		vec4(+0.7f, +0.8f, +0.2f, +1.0f), // Color
		vec3(-1.0f, +0.0f, +0.0f),        // Normal
		vec2(+1.0f, +0.0f),               // UV
		vec3(+0.0f, +0.0f, +1.0f),		  // Tangent
		vec3(-1.0f, +1.0f, -1.0f),        // 13
		vec4(+0.5f, +0.7f, +0.3f, +1.0f), // Color
		vec3(-1.0f, +0.0f, +0.0f),        // Normal
		vec2(+0.0f, +0.0f),               // UV
		vec3(+0.0f, +0.0f, +1.0f),		  // Tangent
		vec3(-1.0f, -1.0f, -1.0f),        // 14
		vec4(+0.4f, +0.7f, +0.7f, +1.0f), // Color
		vec3(-1.0f, +0.0f, +0.0f),        // Normal
		vec2(+0.0f, +1.0f),               // UV
		vec3(+0.0f, +0.0f, +1.0f),		  // Tangent
		vec3(-1.0f, -1.0f, +1.0f),        // 15
		vec4(+0.2f, +0.5f, +1.0f, +1.0f), // Color
		vec3(-1.0f, +0.0f, +0.0f),        // Normal
		vec2(+1.0f, +1.0f),               // UV
		vec3(+0.0f, +0.0f, +1.0f),		  // Tangent

		// Back
		vec3(+1.0f, +1.0f, +1.0f),        // 16
		vec4(+0.6f, +1.0f, +0.7f, +1.0f), // Color
		vec3(+0.0f, +0.0f, +1.0f),        // Normal
		vec2(+1.0f, +0.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent
		vec3(-1.0f, +1.0f, +1.0f),        // 17
		vec4(+0.6f, +0.4f, +0.8f, +1.0f), // Color
		vec3(+0.0f, +0.0f, +1.0f),        // Normal
		vec2(+0.0f, +0.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent
		vec3(-1.0f, -1.0f, +1.0f),        // 18
		vec4(+0.2f, +0.8f, +0.7f, +1.0f), // Color
		vec3(+0.0f, +0.0f, +1.0f),        // Normal
		vec2(+0.0f, +1.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent
		vec3(+1.0f, -1.0f, +1.0f),        // 19
		vec4(+0.2f, +0.7f, +1.0f, +1.0f), // Color
		vec3(+0.0f, +0.0f, +1.0f),        // Normal
		vec2(+1.0f, +1.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent

		// Bottom
		vec3(+1.0f, -1.0f, -1.0f),        // 20
		vec4(+0.8f, +0.3f, +0.7f, +1.0f), // Color
		vec3(+0.0f, -1.0f, +0.0f),        // Normal
		vec2(+1.0f, +1.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent
		vec3(-1.0f, -1.0f, -1.0f),        // 21
		vec4(+0.8f, +0.9f, +0.5f, +1.0f), // Color
		vec3(+0.0f, -1.0f, +0.0f),        // Normal
		vec2(+0.0f, +1.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent
		vec3(-1.0f, -1.0f, +1.0f),        // 22
		vec4(+0.5f, +0.8f, +0.5f, +1.0f), // Color
		vec3(+0.0f, -1.0f, +0.0f),        // Normal
		vec2(+0.0f, +0.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent
		vec3(+1.0f, -1.0f, +1.0f),        // 23
		vec4(+0.9f, +1.0f, +0.2f, +1.0f), // Color
		vec3(+0.0f, -1.0f, +0.0f),        // Normal
		vec2(+1.0f, +0.0f),               // UV
		vec3(+1.0f, +0.0f, +0.0f),		  // Tangent
	};
	unsigned short stackIndices[] = {
		0, 1, 2, 0, 2, 3, // Top
		4, 5, 6, 4, 6, 7, // Front
		8, 9, 10, 8, 10, 11, // Right 
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};

	return copyToShapeData(stackVerts, ARRAY_SIZE(stackVerts), stackIndices, ARRAY_SIZE(stackIndices));
}

ShapeData ShapeGenerator::copyToShapeData(
	VertexPCNUT* verts, size_t numVerts,
	ushort* indices, uint numIndices)
{
	ShapeData ret;
	ret.numVerts = numVerts;
	ret.verts = new VertexPCNUT[ret.numVerts];
	memcpy(ret.verts, verts, sizeof(VertexPCNUT)* numVerts);

	ret.numIndices = numIndices;
	ret.indices = new ushort[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(ushort)* numIndices);
	return ret;
}

ShapeData ShapeGenerator::makeSphere(uint tesselation)
{
	ShapeData ret = makePlaneVerts(tesselation);
	ShapeData ret2 = makePlaneIndices(tesselation);
	ret.indices = ret2.indices;
	ret.numIndices = ret2.numIndices;

	uint dimensions = tesselation + 1;
	const float RADIUS = 1.0f;

	const double CIRCLE = PI * 2;
	const double SLICE_ANGLE = CIRCLE / (dimensions - 1);
	for (size_t col = 0; col < dimensions; col++)
	{
		double phi = -SLICE_ANGLE * col;
		for (size_t row = 0; row < dimensions; row++)
		{
			double theta = -(SLICE_ANGLE / 2.0) * row;
			size_t vertIndex = col * dimensions + row;
			VertexPCNUT& v = ret.verts[vertIndex];
			v.position.x = RADIUS * cos(phi) * sin(theta);
			v.position.y = RADIUS * sin(phi) * sin(theta);
			v.position.z = RADIUS * cos(theta);
			v.normal = glm::normalize(v.position);
		}
	}
	return ret;
}

ShapeData ShapeGenerator::makeTorus(uint tesselation)
{
	ShapeData ret;
	uint dimensions = tesselation * tesselation;
	ret.numVerts = dimensions;
	ret.verts = new VertexPCNUT[ret.numVerts];
	float sliceAngle = 360 / tesselation;
	const float torusRadius = 1.0f;
	const float pipeRadius = 0.5f;
	for (uint round1 = 0; round1 < tesselation; round1++)
	{
		// Generate a circle on the xy plane, then
		// translate then rotate it into position
		glm::mat4 transform =
			glm::rotate(glm::mat4(), round1 * sliceAngle, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::translate(glm::mat4(), glm::vec3(torusRadius, 0.0f, 0.0f));
		glm::mat3 normalTransform = (glm::mat3)transform;
		for (uint round2 = 0; round2 < tesselation; round2++)
		{
			VertexPCNUT& v = ret.verts[round1 * tesselation + round2];
			glm::vec4 glmVert(
				pipeRadius * cos(glm::radians(sliceAngle * round2)),
				pipeRadius * sin(glm::radians(sliceAngle * round2)),
				0,
				1.0f);
			glm::vec4 glmVertPrime = transform * glmVert;
			v.position = (glm::vec3)glmVertPrime;
			v.normal = glm::normalize(normalTransform * (glm::vec3)glmVert);
			v.color = randomColor();
			v.uv = glm::vec2((float)round1 / tesselation, (float)round2 / tesselation);
		}
	}

	ShapeData ret2 = makePlaneUnseamedIndices(tesselation);
	ret.numIndices = ret2.numIndices;
	ret.indices = ret2.indices;
	return ret;
}

ShapeData ShapeGenerator::makeArrow()
{
	using glm::vec2;
	using glm::vec3;
	using glm::vec4;
	VertexPCNUT stackVerts[] =
	{
		// Arrow head
		// Top side of arrow head
		vec3(+0.00f, +0.25f, -0.25f),         // 0
		vec4(+1.00f, +0.00f, +0.00f, +1.00f), // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.50f, +0.25f, -0.25f),         // 1
		vec4(+1.00f, +0.00f, +0.00f, +1.00f), // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.00f, +0.25f, -1.00f),         // 2
		vec4(+1.00f, +0.00f, +0.00f, +1.00f), // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(-0.50f, +0.25f, -0.25f),         // 3
		vec4(+1.00f, +0.00f, +0.00f, +1.00f), // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		// Bottom side of arrow head
		vec3(+0.00f, -0.25f, -0.25f),         // 4
		vec4(+0.00f, +0.00f, +1.00f, +1.00f), // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.50f, -0.25f, -0.25f),         // 5
		vec4(+0.00f, +0.00f, +1.00f, +1.00f), // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.00f, -0.25f, -1.00f),         // 6
		vec4(+0.00f, +0.00f, +1.00f, +1.00f), // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(-0.50f, -0.25f, -0.25f),         // 7
		vec4(+0.00f, +0.00f, +1.00f, +1.00f), // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		// Right side of arrow tip
		vec3(+0.50f, +0.25f, -0.25f),         // 8
		vec4(+0.60f, +1.00f, +0.00f, +1.00f), // Color
		vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.00f, +0.25f, -1.00f),         // 9
		vec4(+0.60f, +1.00f, +0.00f, +1.00f), // Color
		vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.00f, -0.25f, -1.00f),         // 10
		vec4(+0.60f, +1.00f, +0.00f, +1.00f), // Color
		vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.50f, -0.25f, -0.25f),         // 11
		vec4(+0.60f, +1.00f, +0.00f, +1.00f), // Color
		vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		// Left side of arrow tip
		vec3(+0.00f, +0.25f, -1.00f),         // 12
		vec4(+0.00f, +1.00f, +0.00f, +1.00f), // Color
		vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(-0.50f, +0.25f, -0.25f),         // 13
		vec4(+0.00f, +1.00f, +0.00f, +1.00f), // Color
		vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.00f, -0.25f, -1.00f),         // 14
		vec4(+0.00f, +1.00f, +0.00f, +1.00f), // Color
		vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(-0.50f, -0.25f, -0.25f),         // 15
		vec4(+0.00f, +1.00f, +0.00f, +1.00f), // Color
		vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		// Back side of arrow tip
		vec3(-0.50f, +0.25f, -0.25f),         // 16
		vec4(+0.50f, +0.50f, +0.50f, +1.00f), // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.50f, +0.25f, -0.25f),         // 17
		vec4(+0.50f, +0.50f, +0.50f, +1.00f), // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(-0.50f, -0.25f, -0.25f),         // 18
		vec4(+0.50f, +0.50f, +0.50f, +1.00f), // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.50f, -0.25f, -0.25f),         // 19
		vec4(+0.50f, +0.50f, +0.50f, +1.00f), // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		// Top side of back of arrow
		vec3(+0.25f, +0.25f, -0.25f),         // 20
		vec4(+1.00f, +0.00f, +0.00f, +1.00f), // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.25f, +0.25f, +1.00f),         // 21
		vec4(+1.00f, +0.00f, +0.00f, +1.00f), // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(-0.25f, +0.25f, +1.00f),         // 22
		vec4(+1.00f, +0.00f, +0.00f, +1.00f), // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(-0.25f, +0.25f, -0.25f),         // 23
		vec4(+1.00f, +0.00f, +0.00f, +1.00f), // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		// Bottom side of back of arrow
		vec3(+0.25f, -0.25f, -0.25f),         // 24
		vec4(+0.00f, +0.00f, +1.00f, +1.00f), // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.25f, -0.25f, +1.00f),         // 25
		vec4(+0.00f, +0.00f, +1.00f, +1.00f), // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(-0.25f, -0.25f, +1.00f),         // 26
		vec4(+0.00f, +0.00f, +1.00f, +1.00f), // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(-0.25f, -0.25f, -0.25f),         // 27
		vec4(+0.00f, +0.00f, +1.00f, +1.00f), // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		// Right side of back of arrow
		vec3(+0.25f, +0.25f, -0.25f),         // 28
		vec4(+0.60f, +1.00f, +0.00f, +1.00f), // Color
		vec3(+1.00f, +0.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.25f, -0.25f, -0.25f),         // 29
		vec4(+0.60f, +1.00f, +0.00f, +1.00f), // Color
		vec3(+1.00f, +0.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.25f, -0.25f, +1.00f),         // 30
		vec4(+0.60f, +1.00f, +0.00f, +1.00f), // Color
		vec3(+1.00f, +0.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.25f, +0.25f, +1.00f),         // 31
		vec4(+0.60f, +1.00f, +0.00f, +1.00f), // Color
		vec3(+1.00f, +0.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		// Left side of back of arrow
		vec3(-0.25f, +0.25f, -0.25f),         // 32
		vec4(+0.00f, +1.00f, +0.00f, +1.00f), // Color
		vec3(-1.00f, +0.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(-0.25f, -0.25f, -0.25f),         // 33
		vec4(+0.00f, +1.00f, +0.00f, +1.00f), // Color
		vec3(-1.00f, +0.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(-0.25f, -0.25f, +1.00f),         // 34
		vec4(+0.00f, +1.00f, +0.00f, +1.00f), // Color
		vec3(-1.00f, +0.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(-0.25f, +0.25f, +1.00f),         // 35
		vec4(+0.00f, +1.00f, +0.00f, +1.00f), // Color
		vec3(-1.00f, +0.00f, +0.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		// Back side of back of arrow
		vec3(-0.25f, +0.25f, +1.00f),         // 36
		vec4(+0.50f, +0.50f, +0.50f, +1.00f), // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.25f, +0.25f, +1.00f),         // 37
		vec4(+0.50f, +0.50f, +0.50f, +1.00f), // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(-0.25f, -0.25f, +1.00f),         // 38
		vec4(+0.50f, +0.50f, +0.50f, +1.00f), // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
		vec3(+0.25f, -0.25f, +1.00f),         // 39
		vec4(+0.50f, +0.50f, +0.50f, +1.00f), // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec2(+0.00f, +0.00f),                 // UV
		vec3(+0.00f, +0.00f, +0.00f),		  // Tangent
	};

	ushort stackIndices[] = {
		0, 1, 2, // Top
		0, 2, 3,
		4, 6, 5, // Bottom
		4, 7, 6,
		8, 10, 9, // Right side of arrow tip
		8, 11, 10,
		12, 15, 13, // Left side of arrow tip
		12, 14, 15,
		16, 19, 17, // Back side of arrow tip
		16, 18, 19,
		20, 22, 21, // Top side of back of arrow
		20, 23, 22,
		24, 25, 26, // Bottom side of back of arrow
		24, 26, 27,
		28, 30, 29, // Right side of back of arrow
		28, 31, 30,
		32, 33, 34, // Left side of back of arrow
		32, 34, 35,
		36, 39, 37, // Back side of back of arrow
		36, 38, 39,
	};

	return copyToShapeData(stackVerts, ARRAY_SIZE(stackVerts), stackIndices, ARRAY_SIZE(stackIndices));
}

void ShapeGenerator::makeTeapot(
	unsigned int grid, const glm::mat4& lidTransform,
	unsigned int& numVerts,
	float*& verts,
	float*& normals,
	float*& textureCoordinates,
	unsigned short*& indices,
	unsigned int& numIndices)
{
	numVerts = 32 * (grid + 1) * (grid + 1);
	uint faces = grid * grid * 32;
	verts = new float[numVerts * 3];
	normals = new float[numVerts * 3];
	textureCoordinates = new float[numVerts * 2];
	numIndices = faces * 6;
	indices = new unsigned short[numIndices];

	generatePatches(verts, normals, textureCoordinates, indices, grid);
	moveLid(grid, verts, lidTransform);
}

ShapeData ShapeGenerator::makeTeapot(uint tesselation, const glm::mat4& lidTransform)
{
	ShapeData ret;
	float* Vs = NULL;
	float* Ns = NULL;
	float* TCs = NULL;

	makeTeapot(tesselation, lidTransform, ret.numVerts, Vs, Ns, TCs, ret.indices, ret.numIndices);
	ret.verts = new VertexPCNUT[ret.numVerts];
	glm::vec4 white(1, 1, 1, 1);
	glm::mat3 rotator = glm::mat3(glm::rotate(-90.0f, 1.0f, 0.0f, 0.0f));
	for (uint i = 0; i < ret.numVerts; i++)
	{
		VertexPCNUT& v = ret.verts[i];
		v.position.x = Vs[i * 3 + 0];
		v.position.y = Vs[i * 3 + 1];
		v.position.z = Vs[i * 3 + 2];
		v.position = rotator * v.position;
		v.color = white;
		v.uv.x = TCs[i * 2 + 0];
		v.uv.y = TCs[i * 2 + 1];
		v.normal.x = Ns[i * 3 + 0];
		v.normal.y = Ns[i * 3 + 1];
		v.normal.z = Ns[i * 3 + 2];
		v.normal = rotator * v.normal;
	}
	return ret;
}

void ShapeGenerator::generatePatches(float * v, float * n, float * tc, unsigned short* el, int grid) {
	float * B = new float[4 * (grid + 1)];  // Pre-computed Bernstein basis functions
	float * dB = new float[4 * (grid + 1)]; // Pre-computed derivitives of basis functions

	int idx = 0, elIndex = 0, tcIndex = 0;

	// Pre-compute the basis functions  (Bernstein polynomials)
	// and their derivatives
	computeBasisFunctions(B, dB, grid);

	// Build each patch
	// The rim
	buildPatchReflect(0, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The body
	buildPatchReflect(1, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	buildPatchReflect(2, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The lid
	buildPatchReflect(3, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	buildPatchReflect(4, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The bottom
	buildPatchReflect(5, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The handle
	buildPatchReflect(6, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
	buildPatchReflect(7, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
	// The spout
	buildPatchReflect(8, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
	buildPatchReflect(9, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);

	delete[] B;
	delete[] dB;
}

void ShapeGenerator::moveLid(int grid, float *v, mat4 lidTransform) {

	int start = 3 * 12 * (grid + 1) * (grid + 1);
	int end = 3 * 20 * (grid + 1) * (grid + 1);

	for (int i = start; i < end; i += 3)
	{
		glm::vec4 vert = glm::vec4(v[i], v[i + 1], v[i + 2], 1.0f);
		vert = lidTransform * vert;
		v[i] = vert.x;
		v[i + 1] = vert.y;
		v[i + 2] = vert.z;
	}
}

void ShapeGenerator::buildPatchReflect(int patchNum,
	float *B, float *dB,
	float *v, float *n,
	float *tc, unsigned short *el,
	int &index, int &elIndex, int &tcIndex, int grid,
	bool reflectX, bool reflectY)
{
	glm::vec3 patch[4][4];
	glm::vec3 patchRevV[4][4];
	getPatch(patchNum, patch, false);
	getPatch(patchNum, patchRevV, true);

	// Patch without modification
	buildPatch(patch, B, dB, v, n, tc, el,
		index, elIndex, tcIndex, grid, mat3(1.0f), true);

	// Patch reflected in x
	if (reflectX) {
		buildPatch(patchRevV, B, dB, v, n, tc, el,
			index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)), false);
	}

	// Patch reflected in y
	if (reflectY) {
		buildPatch(patchRevV, B, dB, v, n, tc, el,
			index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)), false);
	}

	// Patch reflected in x and y
	if (reflectX && reflectY) {
		buildPatch(patch, B, dB, v, n, tc, el,
			index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)), true);
	}
}

void ShapeGenerator::buildPatch(glm::vec3 patch[][4],
	float *B, float *dB,
	float *v, float *n, float *tc,
	unsigned short *el,
	int &index, int &elIndex, int &tcIndex, int grid, glm::mat3 reflect,
	bool invertNormal)
{
	int startIndex = index / 3;
	float tcFactor = 1.0f / grid;

	for (int i = 0; i <= grid; i++)
	{
		for (int j = 0; j <= grid; j++)
		{
			glm::vec3 pt = reflect * evaluate(i, j, B, patch);
			glm::vec3 norm = reflect * evaluateNormal(i, j, B, dB, patch);
			if (invertNormal)
				norm = -norm;

			v[index] = pt.x;
			v[index + 1] = pt.y;
			v[index + 2] = pt.z;

			n[index] = norm.x;
			n[index + 1] = norm.y;
			n[index + 2] = norm.z;

			tc[tcIndex] = i * tcFactor;
			tc[tcIndex + 1] = j * tcFactor;

			index += 3;
			tcIndex += 2;
		}
	}

	for (int i = 0; i < grid; i++)
	{
		int iStart = i * (grid + 1) + startIndex;
		int nextiStart = (i + 1) * (grid + 1) + startIndex;
		for (int j = 0; j < grid; j++)
		{
			el[elIndex] = iStart + j;
			el[elIndex + 1] = nextiStart + j + 1;
			el[elIndex + 2] = nextiStart + j;

			el[elIndex + 3] = iStart + j;
			el[elIndex + 4] = iStart + j + 1;
			el[elIndex + 5] = nextiStart + j + 1;

			elIndex += 6;
		}
	}
}

#include "TeapotData.h"

void ShapeGenerator::getPatch(int patchNum, glm::vec3 patch[][4], bool reverseV)
{
	for (int u = 0; u < 4; u++) {          // Loop in u direction
		for (int v = 0; v < 4; v++) {     // Loop in v direction
			if (reverseV) {
				patch[u][v] = glm::vec3(
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][0],
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][1],
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][2]
					);
			}
			else {
				patch[u][v] = glm::vec3(
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][0],
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][1],
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][2]
					);
			}
		}
	}
}

void ShapeGenerator::computeBasisFunctions(float * B, float * dB, int grid) {
	float inc = 1.0f / grid;
	for (int i = 0; i <= grid; i++)
	{
		float t = i * inc;
		float tSqr = t * t;
		float oneMinusT = (1.0f - t);
		float oneMinusT2 = oneMinusT * oneMinusT;

		B[i * 4 + 0] = oneMinusT * oneMinusT2;
		B[i * 4 + 1] = 3.0f * oneMinusT2 * t;
		B[i * 4 + 2] = 3.0f * oneMinusT * tSqr;
		B[i * 4 + 3] = t * tSqr;

		dB[i * 4 + 0] = -3.0f * oneMinusT2;
		dB[i * 4 + 1] = -6.0f * t * oneMinusT + 3.0f * oneMinusT2;
		dB[i * 4 + 2] = -3.0f * tSqr + 6.0f * t * oneMinusT;
		dB[i * 4 + 3] = 3.0f * tSqr;
	}
}

glm::vec3 ShapeGenerator::evaluate(int gridU, int gridV, float *B, glm::vec3 patch[][4])
{
	glm::vec3 p(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			p += patch[i][j] * B[gridU * 4 + i] * B[gridV * 4 + j];
		}
	}
	return p;
}

glm::vec3 ShapeGenerator::evaluateNormal(int gridU, int gridV, float *B, float *dB, glm::vec3 patch[][4])
{
	glm::vec3 du(0.0f, 0.0f, 0.0f);
	glm::vec3 dv(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			du += patch[i][j] * dB[gridU * 4 + i] * B[gridV * 4 + j];
			dv += patch[i][j] * B[gridU * 4 + i] * dB[gridV * 4 + j];
		}
	}
	return glm::normalize(glm::cross(du, dv));
}



/*#include "ShapeGenerator.h"
#include <glm\glm.hpp>
#include <random>
#include <Primitives\Vertex.h>
#include <glm\gtx\transform.hpp>

#define PI 3.1415927

#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

glm::vec4 randomColour()
{
	glm::vec4 ret;
	ret.x = rand() / (float)RAND_MAX;
	ret.y = rand() / (float)RAND_MAX;
	ret.z = rand() / (float)RAND_MAX;
	ret.w = 1;
	return ret;
}

ShapeData ShapeGenerator::makePlaneVerts(GLuint dimensions)
{
	ShapeData ret;
	GLuint vertsPerDimension = dimensions + 1; // + 1 for last row/col of verts
	ret.numVertices = vertsPerDimension * vertsPerDimension;
	ret.vertices = new Vertex[ret.numVertices];
	float half = dimensions / 2.0f;
	glm::vec3 runner(-half, 0.0f, -half);
	for (int i = 0; i < vertsPerDimension; i++)
	{
		for (int j = 0; j < vertsPerDimension; j++)
		{
			Vertex& thisVert = ret.vertices[i * vertsPerDimension + j];
			thisVert.position = runner;
			thisVert.colour = randomColour();
			thisVert.normal = glm::vec3(0, 1, 0);
			runner.x++;
		}
		runner.x = -half;
		runner.z++;
	}
	return ret;
}

ShapeData ShapeGenerator::makePlaneIndices(GLuint dimensions)
{
	ShapeData ret;
	GLuint vertsPerDimension = dimensions + 1;
	ret.numIndices = dimensions * dimensions * 2 * 3; // 2 triangles per square, 3 indices per triangle
	ret.indices = new unsigned short[ret.numIndices];
	int runner = 0;
	for (int row = 0; row < dimensions; row++)
	{
		for (int col = 0; col < dimensions; col++)
		{
			ret.indices[runner++] = vertsPerDimension * row + col;
			ret.indices[runner++] = vertsPerDimension * row + col + vertsPerDimension;
			ret.indices[runner++] = vertsPerDimension * row + col + vertsPerDimension + 1;

			ret.indices[runner++] = vertsPerDimension * row + col;
			ret.indices[runner++] = vertsPerDimension * row + col + vertsPerDimension + 1;
			ret.indices[runner++] = vertsPerDimension * row + col + 1;
		}
	}
	assert(runner = ret.numIndices);
	return ret;
}

ShapeData ShapeGenerator::makePlane(GLuint dimensions)
{
	ShapeData ret = makePlaneVerts(dimensions);
	ShapeData ret2 = makePlaneIndices(dimensions);
	ret.numIndices = ret2.numIndices;
	ret.indices = ret2.indices;
	return ret;
}

ShapeData ShapeGenerator::generateNormals(const ShapeData& data)
{
	ShapeData ret;
	ret.numVertices = data.numVertices * 2;
	ret.vertices = new Vertex[ret.numVertices];
	glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < data.numVertices; i++)
	{
		GLuint vertIndex = i * 2;
		Vertex& v1 = ret.vertices[vertIndex];
		Vertex& v2 = ret.vertices[vertIndex + 1];
		const Vertex& sourceVertex = data.vertices[i];
		v1.position = sourceVertex.position;
		v2.position = sourceVertex.position + sourceVertex.normal;
		v1.colour = v2.colour = white;
	}

	ret.numIndices = ret.numVertices;
	ret.indices = new GLushort[ret.numIndices];
	for (int i = 0; i < ret.numIndices; i++)
	{
		ret.indices[i] = i;
	}
	return ret;
}

ShapeData ShapeGenerator::makeSphere(GLuint tesselation)
{
	ShapeData ret = makePlaneVerts(tesselation);
	ShapeData ret2 = makePlaneIndices(tesselation);
	ret.indices = ret2.indices;
	ret.numIndices = ret2.numIndices;

	uint dimensions = tesselation + 1;
	const float RADIUS = 1.0f;

	const double CIRCLE = PI * 2;
	const double SLICE_ANGLE = CIRCLE / (dimensions - 1);
	for (size_t col = 0; col < dimensions; col++)
	{
		double phi = -SLICE_ANGLE * col;
		for (size_t row = 0; row < dimensions; row++)
		{
			double theta = -(SLICE_ANGLE / 2.0) * row;
			size_t vertIndex = col * dimensions + row;
			Vertex& v = ret.vertices[vertIndex];
			v.position.x = RADIUS * cos(phi) * sin(theta);
			v.position.y = RADIUS * sin(phi) * sin(theta);
			v.position.z = RADIUS * cos(theta);
			v.normal = glm::normalize(v.position);
		}
	}
	return ret;
}

ShapeData ShapeGenerator::makeTorus(GLuint tesselation)
{
	ShapeData ret;
	uint dimensions = tesselation * tesselation;
	ret.numVertices = dimensions;
	ret.vertices = new Vertex[ret.numVertices];
	float sliceAngle = 360 / tesselation;
	const float torusRadius = 1.0f;
	const float pipeRadius = 0.5f;
	for (uint round1 = 0; round1 < tesselation; round1++)
	{
		// Generate a circle on the xy plane, then
		// translate then rotate it into position
		glm::mat4 transform =
			glm::rotate(glm::mat4(), round1 * sliceAngle, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::translate(glm::mat4(), glm::vec3(torusRadius, 0.0f, 0.0f));
		glm::mat3 normalTransform = (glm::mat3)transform;
		for (uint round2 = 0; round2 < tesselation; round2++)
		{
			Vertex& v = ret.vertices[round1 * tesselation + round2];
			glm::vec4 glmVert(
				pipeRadius * cos(glm::radians(sliceAngle * round2)),
				pipeRadius * sin(glm::radians(sliceAngle * round2)),
				0,
				1.0f);
			glm::vec4 glmVertPrime = transform * glmVert;
			v.position = (glm::vec3)glmVertPrime;
			v.normal = glm::normalize(normalTransform * (glm::vec3)glmVert);
			v.colour = randomColour();
		}
	}

	ShapeData ret2 = makePlaneUnseamedIndices(tesselation);
	ret.numIndices = ret2.numIndices;
	ret.indices = ret2.indices;
	return ret;
}

ShapeData ShapeGenerator::makePlaneUnseamedIndices(GLuint tesselation)
{
	ShapeData ret;
	GLuint dimensions = tesselation * tesselation;
	ret.numIndices = dimensions * 2 * 3; // 2 triangles per square, 3 indices per triangle
	ret.indices = new unsigned short[ret.numIndices];
	int runner = 0;
	for (int row = 0; row < tesselation; row++)
	{
		// This code is crap but works, and I'm not in the mood right now to clean it up
		for (int col = 0; col < tesselation; col++)
		{
			// Bottom left triangle
			ret.indices[runner++] = tesselation * row + col;
			// One row down unless it's the bottom row, 
			ret.indices[runner++] = (row + 1 == tesselation ? 0 : tesselation * row + tesselation) + col;
			// Move to vert right of this one unless it's the last vert,
			// which we connect to the first vert in the row
			// the % dimensions at the end accounts for the last row hooking to the first row
			ret.indices[runner++] = (tesselation * row + col + tesselation + (col + 1 == tesselation ? -tesselation + 1 : 1)) % dimensions;

			// Upper right triangle
			ret.indices[runner++] = tesselation * row + col;
			if (col + 1 == tesselation && row + 1 == tesselation)
			{
				// Very last vert
				ret.indices[runner++] = 0;
			}
			else if (col + 1 == tesselation)
			{
				// Last vert on this row
				// Check if we need to connect it to zeroeth row or the next row
				if (row + 1 == tesselation)
				{
					// Tie to zeroeth row
					ret.indices[runner++] = col + 1;
				}
				else
				{
					// Tie to next row
					ret.indices[runner++] = tesselation * row + col + 1;
				}
			}
			else
			{
				// Regular interior vert
				// the % dimensions at the end accounts for the last row hooking to the first row
				ret.indices[runner++] = (tesselation * row + col + tesselation + 1) % dimensions;
			}
			ret.indices[runner++] = tesselation * row + col + (col + 1 == tesselation ? -col : 1);
		}
	}
	return ret;
}*/

/*ShapeData ShapeGenerator::makeCube()
{
	ShapeData ret;

	Vertex verts[] =
	{
		glm::vec3(-1.0f, +1.0f, +1.0f), // 0
		glm::vec4(+1.0f, +0.0f, +0.0f, +1.0f), // Colour
		glm::vec3(+1.0f, +1.0f, +1.0f), // 1
		glm::vec4(+0.0f, +1.0f, +0.0f, +1.0f), // Colour
		glm::vec4(+1.0f, +1.0f, -1.0f), // 2
		glm::vec4(+0.0f, +0.0f, +1.0f, +1.0f), // Colour
		glm::vec3(-1.0f, +1.0f, -1.0f), // 3
		glm::vec3(+1.0f, +1.0f, +1.0f, +1.0f), // Colour

		glm::vec3(-1.0f, +1.0f, -1.0f), // 4
		glm::vec3(+1.0f, +0.0f, +1.0f, +1.0f), // Colour
		glm::vec3(+1.0f, +1.0f, -1.0f), // 5
		glm::vec3(+0.0f, +0.5f, +0.2f, +1.0f), // Colour
		glm::vec3(+1.0f, -1.0f, -1.0f), // 6
		glm::vec3(+0.8f, +0.6f, +0.4f, +1.0f), // Colour
		glm::vec3(-1.0f, -1.0f, -1.0f), // 7
		glm::vec3(+0.3f, +1.0f, +0.5f, +1.0f), // Colour

		glm::vec3(+1.0f, +1.0f, -1.0f), // 8
		glm::vec3(+0.2f, +0.5f, +0.2f, +1.0f), // Colour
		glm::vec3(+1.0f, +1.0f, +1.0f), // 9
		glm::vec3(+0.9f, +0.3f, +0.7f, +1.0f), // Colour
		glm::vec3(+1.0f, -1.0f, +1.0f), // 10
		glm::vec3(+0.3f, +0.7f, +0.5f, +1.0f), // Colour
		glm::vec3(+1.0f, -1.0f, -1.0f), // 11
		glm::vec3(+0.5f, +0.7f, +0.5f, +1.0f), // Colour

		glm::vec3(-1.0f, +1.0f, +1.0f), // 12
		glm::vec3(+0.7f, +0.8f, +0.2f, +1.0f), // Colour
		glm::vec3(-1.0f, +1.0f, -1.0f), // 13
		glm::vec3(+0.5f, +0.7f, +0.3f, +1.0f), // Colour
		glm::vec3(-1.0f, -1.0f, -1.0f), // 14
		glm::vec3(+0.4f, +0.7f, +0.7f, +1.0f), // Colour
		glm::vec3(-1.0f, -1.0f, +1.0f), // 15
		glm::vec3(+0.2f, +0.5f, +1.0f, +1.0f), // Colour

		glm::vec3(+1.0f, +1.0f, +1.0f), // 16
		glm::vec3(+0.6f, +1.0f, +0.7f, +1.0f), // Colour
		glm::vec3(-1.0f, +1.0f, +1.0f), // 17
		glm::vec3(+0.6f, +0.4f, +0.8f, +1.0f), // Colour
		glm::vec3(-1.0f, -1.0f, +1.0f), // 18
		glm::vec3(+0.2f, +0.8f, +0.7f, +1.0f), // Colour
		glm::vec3(+1.0f, -1.0f, +1.0f), // 19
		glm::vec3(+0.2f, +0.7f, +1.0f, +1.0f), // Colour

		glm::vec3(+1.0f, -1.0f, -1.0f), // 20
		glm::vec3(+0.8f, +0.3f, +0.7f, +1.0f), // Colour
		glm::vec3(-1.0f, -1.0f, -1.0f), // 21
		glm::vec3(+0.8f, +0.9f, +0.5f, +1.0f), // Colour
		glm::vec3(-1.0f, -1.0f, +1.0f), // 22
		glm::vec3(+0.5f, +0.8f, +0.5f, +1.0f), // Colour
		glm::vec3(+1.0f, -1.0f, +1.0f), // 23

		/*glm::vec3(+0.0f, +1.0f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),

		glm::vec3(-1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +1.0f, +0.0f),

		glm::vec3(+1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),*/
	/*};

		ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
		ret.vertices = new Vertex[ret.numVertices];
		memcpy(ret.vertices, verts, sizeof(verts));

		//GLshort indices[] = { 0, 1, 2 };

		GLushort indices[] =
		{
		0, 1, 2, 0, 2, 3, // Top
		4, 5, 6, 4, 6, 7, // Front
		8, 9, 10, 8, 10, 11, // Right
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
		};

		ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
		ret.indices = new GLushort[ret.numIndices];
		memcpy(ret.indices, indices, sizeof(indices));

		return ret;
}*/