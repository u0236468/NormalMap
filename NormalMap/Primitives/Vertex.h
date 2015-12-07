#ifndef ENGINE_VERTEX_H
#define ENGINE_VERTEX_H
#include <glm\glm.hpp>
class VertexLayoutInfo;

struct VertexPC
{
	glm::vec3 position;
	glm::vec4 color;
};

struct VertexPNUT
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 tangent;
};

struct VertexPCNUT
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
};

enum VertexOffsets
{
	VO_PC_POSITION = 0,
	VO_PC_COLOR = VO_PC_POSITION + sizeof(glm::vec3),
	VO_PC_STRIDE = sizeof(VertexPC),

	VO_PNUT_POSITION = 0,
	VO_PNUT_NORMAL = VO_PNUT_POSITION + sizeof(glm::vec3),
	VO_PNUT_UV = VO_PNUT_NORMAL + sizeof(glm::vec3),
	VO_PNUT_TANGENT = VO_PNUT_UV + sizeof(glm::vec2),
	VO_PNUT_STRIDE = sizeof(VertexPNUT),

	VO_PCNUT_POSITION = 0,
	VO_PCNUT_COLOR = sizeof(glm::vec3),
	VO_PCNUT_NORMAL = VO_PCNUT_COLOR + sizeof(glm::vec4),
	VO_PCNUT_UV = VO_PCNUT_NORMAL + sizeof(glm::vec3),
	VO_PCNUT_TANGENT = VO_PCNUT_UV + sizeof(glm::vec2),
	VO_PCNUT_STRIDE = sizeof(VertexPCNUT),
};

#endif


/*#pragma once
#include <glm\glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec4 colour;
	glm::vec3 normal;
};*/