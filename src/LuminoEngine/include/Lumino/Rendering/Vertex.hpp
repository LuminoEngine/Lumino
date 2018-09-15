
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

} // namespace ln
