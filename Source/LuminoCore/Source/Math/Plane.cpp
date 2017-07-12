
#include <math.h>
#include <cmath>
#include "../../include/Lumino/Math/Vector3.h"
#include "../../include/Lumino/Math/Vector4.h"
#include "../../include/Lumino/Math/Matrix.h"
#include "../../include/Lumino/Math/Geometries.h"
#include "../../include/Lumino/Math/Plane.h"
#include "Asm.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Plane
//==============================================================================

//------------------------------------------------------------------------------
Plane::Plane()
	: Normal(0.0f, 0.0f, 0.0f)
	, D(0.0f)
{
}

//------------------------------------------------------------------------------
Plane::Plane(float a, float b, float c, float d)
	: Normal(a, b, c)
	, D(d)
{
}

//------------------------------------------------------------------------------
Plane::Plane(const Vector3& normal, float d)
	: Normal(normal)
	, D(d)
{
}

//------------------------------------------------------------------------------
Plane::Plane(const Vector3& normal)
	: Plane(Vector3::Zero, normal)
{
}

//------------------------------------------------------------------------------
Plane::Plane(const Vector3& p1, const Vector3& normal)
	: Normal(normal)
	, D(-Vector3::dot(normal, p1))
{
}
	
//------------------------------------------------------------------------------
Plane::Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3)
{
	Normal = Vector3::cross((point2 - point1), (point3 - point1));
	Normal.normalize();

	D = -Vector3::dot(point1, Normal);
}

//------------------------------------------------------------------------------
void Plane::normalize()
{
	float t = 1.0f / Asm::sqrt((Normal.x * Normal.x) + (Normal.y * Normal.y) + (Normal.z * Normal.z));
	Normal.x *= t;
	Normal.y *= t;
	Normal.z *= t;
	D *= t;
}

//------------------------------------------------------------------------------
bool Plane::intersects(const Vector3& start, const Vector3& end, Vector3* point) const
{
	Vector3 p = Vector3(Normal.x * D, Normal.y * D, Normal.z * D);
	Vector3 pa = p - start;
	Vector3 pb = p - end;
	float dot_pa = Vector3::dot(pa, Normal);
	float dot_pb = Vector3::dot(pb, Normal);

	// 交差判定
	if (dot_pa == 0.0 && dot_pb == 0.0)
	{
		// 両端が平面上にあり、交点を計算できない。
		return false;
	}
	else if (
		(dot_pa >= 0.0 && dot_pb <= 0.0) ||
		(dot_pa <= 0.0 && dot_pb >= 0.0))
	{
		// 内積の片方がプラスで片方がマイナスなので、交差している
	}
	else
	{
		// 交差していない
		return false;
	}

	if (point != nullptr)
	{
		Vector3 direction = end - start;
		float dot = Vector3::dot(Normal, direction);
		float t = (D + Vector3::dot(Normal, start)) / dot;	// 交点とAの距離 : 交点とBの距離
		(*point) = start - (t * direction);
	}
	return true;
}

//------------------------------------------------------------------------------
bool Plane::intersects(const Ray& ray, Vector3* point) const
{
	float dot = Vector3::dot(Normal, ray.direction);
	if (std::abs(dot) > 0.0001f)
	{
		if (point != nullptr)
		{
			float t = (D + Vector3::dot(Normal, ray.origin)) / dot;
			//if (t >= 0) return true;	// 表面側なら +、裏面なら -
			(*point) = ray.origin - (t * ray.direction);
		}
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
void Plane::transform(const Matrix& mat)
{
	float x = Normal.x;
	float y = Normal.y;
	float z = Normal.z;
	float d = D;
	Matrix t = Matrix::makeInverse(mat);

	Normal.x = (((x * t.m11) + (y * t.m12)) + (z * t.m13)) + (d * t.m14);
	Normal.y = (((x * t.m21) + (y * t.m22)) + (z * t.m23)) + (d * t.m24);
	Normal.z = (((x * t.m31) + (y * t.m32)) + (z * t.m33)) + (d * t.m34);
	D = (((x * t.m41) + (y * t.m42)) + (z * t.m43)) + (d * t.m44);
}

//------------------------------------------------------------------------------
void Plane::print(const char* format, FILE* stream) const
{
	if (!format) {
		format = "%f, %f, %f, %f\n";
	}
	if (!stream) {
		stream = stdout;
	}
	fprintf(stream, format, Normal.x, Normal.y, Normal.z, D);
}

//------------------------------------------------------------------------------
Plane Plane::normalize(const Plane& plane)
{
	float t = 1.0f / Asm::sqrt((plane.Normal.x * plane.Normal.x) + (plane.Normal.y * plane.Normal.y) + (plane.Normal.z * plane.Normal.z));
	return Plane(
		plane.Normal.x * t,
		plane.Normal.y * t,
		plane.Normal.z * t,
		plane.D * t);
}

//------------------------------------------------------------------------------
float Plane::dot(const Plane& plane, const Vector4& vec)
{
	return (plane.Normal.x * vec.x) + (plane.Normal.y * vec.y) + (plane.Normal.z * vec.z) + (plane.D * vec.w);
}

//------------------------------------------------------------------------------
float Plane::dotCoord(const Plane& plane, const Vector3& vec)
{
	return (plane.Normal.x * vec.x) + (plane.Normal.y * vec.y) + (plane.Normal.z * vec.z) + plane.D;
}

//------------------------------------------------------------------------------
float Plane::dotNormal(const Plane& plane, const Vector3& vec)
{
	return (plane.Normal.x * vec.x) + (plane.Normal.y * vec.y) + (plane.Normal.z * vec.z);
}

//------------------------------------------------------------------------------
Plane Plane::transform(const Plane& plane, const Matrix& mat)
{
	float x = plane.Normal.x;
	float y = plane.Normal.y;
	float z = plane.Normal.z;
	float d = plane.D;
	Matrix t = Matrix::makeInverse(mat);

	return Plane(
		(((x * t.m11) + (y * t.m12)) + (z * t.m13)) + (d * t.m14),
		(((x * t.m21) + (y * t.m22)) + (z * t.m23)) + (d * t.m24),
		(((x * t.m31) + (y * t.m32)) + (z * t.m33)) + (d * t.m34),
		(((x * t.m41) + (y * t.m42)) + (z * t.m43)) + (d * t.m44));
}

LN_NAMESPACE_END
