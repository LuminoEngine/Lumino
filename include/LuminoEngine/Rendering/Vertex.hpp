
#pragma once
#include "../Graphics/ColorStructs.hpp"

namespace ln {

struct Vertex
{
    static const Vertex Default;

	Vector3 position;
	Vector3 normal;
	Vector2 uv;
	Color color;
};

struct VertexBlendWeight
{
	float		weights[4];
	float		indices[4];
};

struct VertexAdditionalUV
{
	Vector4		uv[4];
};

struct VertexSdefInfo
{
	Vector4		sdefC;
	Vector3		sdefR0;
	Vector3		sdefR1;
};

struct VertexMmdExtra
{
	float		edgeWeight;
	float		index;
};

} // namespace ln
