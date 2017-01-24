
#pragma once
#include "Color.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief		
*/
struct Vertex
{
	Vector3	position;
	Vector2	uv;
	Vector3	normal;
	Color	color;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
