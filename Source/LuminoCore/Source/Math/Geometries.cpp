
#include "../../include/Lumino/Math/Vector3.h"
#include "../../include/Lumino/Math/Geometries.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Ray
//==============================================================================

//------------------------------------------------------------------------------
Ray::Ray()
	: origin(0.0f, 0.0f, 0.0f)
	, direction(0.0f, 0.0f, 0.0f)
{
}

//------------------------------------------------------------------------------
Ray::Ray(const Vector3& origin_, const Vector3& direction_)
	: origin(origin_)
	, direction(direction_)
{
}


//==============================================================================
// Box
//==============================================================================

//------------------------------------------------------------------------------
Box::Box()
	: center(Vector3::Zero)
	, width(0)
	, height(0)
	, depth(0)
{
}

//------------------------------------------------------------------------------
Box::Box(float sizeXYZ)
	: center(Vector3::Zero)
	, width(sizeXYZ)
	, height(sizeXYZ)
	, depth(sizeXYZ)
{
}

//------------------------------------------------------------------------------
Box::Box(const Vector3& min, const Vector3& max)
{
	width = max.x - min.x;
	center.x = min.x + (width / 2);
	height = max.y - min.y;
	center.y = min.y + (height / 2);
	depth = max.z - min.z;
	center.z = min.z + (depth / 2);
}

//------------------------------------------------------------------------------
void Box::GetMinMax(Vector3* outMin, Vector3* outMax) const
{
	float hw = width / 2;
	float hh = height / 2;
	float hd = depth / 2;

	if (outMin != nullptr)
	{
		outMin->x = center.x - hw;
		outMin->y = center.y - hh;
		outMin->z = center.z - hd;
	}
	if (outMax != nullptr)
	{
		outMax->x = center.x + hw;
		outMax->y = center.y + hh;
		outMax->z = center.z + hd;
	}
}

//==============================================================================
// OrientedBox
//==============================================================================

//------------------------------------------------------------------------------
OrientedBox::OrientedBox()
	: center(Vector3::Zero)
	, axisX(Vector3::UnitX)
	, axisY(Vector3::UnitY)
	, axisZ(Vector3::UnitZ)
	, extentX(0)
	, extentY(0)
	, extentZ(0)
{
}

//------------------------------------------------------------------------------
OrientedBox::OrientedBox(float sizeXYZ)
	: center(Vector3::Zero)
	, axisX(Vector3::UnitX)
	, axisY(Vector3::UnitY)
	, axisZ(Vector3::UnitZ)
	, extentX(sizeXYZ)
	, extentY(sizeXYZ)
	, extentZ(sizeXYZ)
{
}

LN_NAMESPACE_END
