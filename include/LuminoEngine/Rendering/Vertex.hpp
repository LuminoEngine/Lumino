
#pragma once
#include "../Graphics/ColorStructs.hpp"

namespace ln {

struct Vertex
{
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

} // namespace ln
