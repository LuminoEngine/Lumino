
#include <LuminoCore/Math/Math.hpp>
#include <LuminoCore/Math/Vector3.hpp>
#include <LuminoCore/Math/Matrix.hpp>
#include <LuminoCore/Math/Geometries.hpp>

namespace ln {

//==============================================================================
// Ray

Ray::Ray()
    : origin(0.0f, 0.0f, 0.0f)
    , direction(0.0f, 0.0f, 0.0f)
	, distance(Math::Inf)
{
}

Ray::Ray(const Vector3& origin_, const Vector3& direction_)
    : origin(origin_)
    , direction(direction_)
	, distance(Math::Inf)
{
}

Ray::Ray(const Vector3& origin_, const Vector3& direction_, float distance_)
	: origin(origin_)
	, direction(direction_)
	, distance(distance_)
{
}

//==============================================================================
// Box

Box::Box()
    : center(Vector3::Zero)
    , width(0)
    , height(0)
    , depth(0)
{
}

Box::Box(float sizeXYZ)
    : center(Vector3::Zero)
    , width(sizeXYZ)
    , height(sizeXYZ)
    , depth(sizeXYZ)
{
}

Box::Box(const Vector3& center_, float sizeXYZ)
    : center(center_)
    , width(sizeXYZ)
    , height(sizeXYZ)
    , depth(sizeXYZ)
{
}

Box::Box(const Vector3& center_, float width_, float height_, float depth_)
    : center(center_)
    , width(width_)
    , height(height_)
    , depth(depth_)
{
}

Box::Box(const Vector3& min, const Vector3& max)
{
    width = max.x - min.x;
    center.x = min.x + (width / 2);
    height = max.y - min.y;
    center.y = min.y + (height / 2);
    depth = max.z - min.z;
    center.z = min.z + (depth / 2);
}

Vector3 Box::minPoint() const
{
    float hw = width / 2;
    float hh = height / 2;
    float hd = depth / 2;
    return Vector3(center.x - hw, center.y - hh, center.z - hd);
}

Vector3 Box::maxPoint() const
{
    float hw = width / 2;
    float hh = height / 2;
    float hd = depth / 2;
    return Vector3(center.x + hw, center.y + hh, center.z + hd);
}

void Box::getMinMax(Vector3* outMin, Vector3* outMax) const
{
    float hw = width / 2;
    float hh = height / 2;
    float hd = depth / 2;

    if (outMin != nullptr) {
        outMin->x = center.x - hw;
        outMin->y = center.y - hh;
        outMin->z = center.z - hd;
    }
    if (outMax != nullptr) {
        outMax->x = center.x + hw;
        outMax->y = center.y + hh;
        outMax->z = center.z + hd;
    }
}

// http://dev.theomader.com/transform-bounding-boxes/
Box Box::transform(const Box& box, const Matrix& mat)
{
	Vector3 boxMin, boxMax;
	box.getMinMax(&boxMin, &boxMax);

	auto xa = mat.right() * boxMin.x;
	auto xb = mat.right() * boxMax.x;

	auto ya = mat.up() * boxMin.y;
	auto yb = mat.up() * boxMax.y;

	auto za = mat.front() * boxMin.z;
	auto zb = mat.front() * boxMax.z;

	return Box(
		Vector3::min(xa, xb) + Vector3::min(ya, yb) + Vector3::min(za, zb) + mat.position(),
		Vector3::max(xa, xb) + Vector3::max(ya, yb) + Vector3::max(za, zb) + mat.position());
}

//==============================================================================
// OrientedBox

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

} // namespace ln
