
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
	Vector4 tangent;	// 接線ベクトル, 従法線の符号
};

struct VertexBlendWeight
{
    static const VertexBlendWeight Default;

	float		indices[4];
	float		weights[4];
};

struct VertexMorphTarget
{
	Vector3 position;
	Vector3 normal;
	Vector3 tangent;
};

// TEXCOORD_1,TEXCOORD_2,TEXCOORD_3
struct VertexAdditionalUV
{
	Vector4		uv[3];
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
