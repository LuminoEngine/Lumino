
#include <math.h>
#include <cmath>
#include <LuminoCore/Math/Math.hpp>
#include <LuminoCore/Math/Vector3.hpp>
#include <LuminoCore/Math/Vector4.hpp>
#include <LuminoCore/Math/Matrix.hpp>
#include <LuminoCore/Math/Geometries.hpp>
#include <LuminoCore/Math/Plane.hpp>
#include "Asm.h"

namespace ln {

//==============================================================================
// Plane

Plane::Plane()
    : normal(0.0f, 0.0f, 0.0f)
    , distance(0.0f)
{
}

Plane::Plane(float a, float b, float c, float d)
    : normal(a, b, c)
    , distance(d)
{
}

Plane::Plane(const Vector3& normal, float d)
    : normal(normal)
    , distance(d)
{
}

Plane::Plane(const Vector3& normal)
    : Plane(Vector3::Zero, normal)
{
}

Plane::Plane(const Vector3& p1, const Vector3& normal)
    : normal(normal)
    , distance(-Vector3::dot(normal, p1))
{
}

Plane::Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3)
{
    normal = Vector3::cross((point2 - point1), (point3 - point1));
    normal.mutatingNormalize();

    distance = -Vector3::dot(point1, normal);
}

void Plane::normalize()
{
    float t = 1.0f / Asm::sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));
    normal.x *= t;
    normal.y *= t;
    normal.z *= t;
    distance *= t;
}

bool Plane::intersects(const Vector3& start, const Vector3& end, Vector3* point) const
{
    Vector3 p = Vector3(normal.x * distance, normal.y * distance, normal.z * distance);
    Vector3 pa = p - start;
    Vector3 pb = p - end;
    float dot_pa = Vector3::dot(pa, normal);
    float dot_pb = Vector3::dot(pb, normal);

    // 交差判定
    if (dot_pa == 0.0 && dot_pb == 0.0) {
        // 両端が平面上にあり、交点を計算できない。
        return false;
    } else if (
        (dot_pa >= 0.0 && dot_pb <= 0.0) ||
        (dot_pa <= 0.0 && dot_pb >= 0.0)) {
        // 内積の片方がプラスで片方がマイナスなので、交差している
    } else {
        // 交差していない
        return false;
    }

    if (point != nullptr) {
        Vector3 direction = end - start;
        float dot = Vector3::dot(normal, direction);
        float t = (distance + Vector3::dot(normal, start)) / dot; // 交点とAの距離 : 交点とBの距離
        (*point) = start - (t * direction);
    }
    return true;
}

bool Plane::intersects(const Ray& ray, Vector3* point) const
{
	if (Math::isInf(ray.distance)) {
		float dot = Vector3::dot(normal, ray.direction);
		if (std::abs(dot) > 0.0001f) {
			if (point != nullptr) {
				float t = (distance + Vector3::dot(normal, ray.origin)) / dot;
				//if (t >= 0) return true;	// 表面側なら +、裏面なら -
				(*point) = ray.origin - (t * ray.direction);
			}
			return true;
		}
		return false;
	}
	else {
		return intersects(ray.origin, ray.endPoint(), point);
	}
}

void Plane::transform(const Matrix& mat)
{
    float x = normal.x;
    float y = normal.y;
    float z = normal.z;
    float d = distance;
    Matrix t = Matrix::makeInverse(mat);

    normal.x = (((x * t.m11) + (y * t.m12)) + (z * t.m13)) + (d * t.m14);
    normal.y = (((x * t.m21) + (y * t.m22)) + (z * t.m23)) + (d * t.m24);
    normal.z = (((x * t.m31) + (y * t.m32)) + (z * t.m33)) + (d * t.m34);
    distance = (((x * t.m41) + (y * t.m42)) + (z * t.m43)) + (d * t.m44);
}

Plane Plane::normalize(const Plane& plane)
{
    float t = 1.0f / Asm::sqrt((plane.normal.x * plane.normal.x) + (plane.normal.y * plane.normal.y) + (plane.normal.z * plane.normal.z));
    return Plane(
        plane.normal.x * t,
        plane.normal.y * t,
        plane.normal.z * t,
        plane.distance * t);
}

float Plane::dot(const Plane& plane, const Vector4& vec)
{
    return (plane.normal.x * vec.x) + (plane.normal.y * vec.y) + (plane.normal.z * vec.z) + (plane.distance * vec.w);
}

float Plane::dotCoord(const Plane& plane, const Vector3& vec)
{
    return (plane.normal.x * vec.x) + (plane.normal.y * vec.y) + (plane.normal.z * vec.z) + plane.distance;
}

float Plane::dotNormal(const Plane& plane, const Vector3& vec)
{
    return (plane.normal.x * vec.x) + (plane.normal.y * vec.y) + (plane.normal.z * vec.z);
}

Plane Plane::transform(const Plane& plane, const Matrix& mat)
{
    float x = plane.normal.x;
    float y = plane.normal.y;
    float z = plane.normal.z;
    float d = plane.distance;
    Matrix t = Matrix::makeInverse(mat);

    return Plane(
        (((x * t.m11) + (y * t.m12)) + (z * t.m13)) + (d * t.m14),
        (((x * t.m21) + (y * t.m22)) + (z * t.m23)) + (d * t.m24),
        (((x * t.m31) + (y * t.m32)) + (z * t.m33)) + (d * t.m34),
        (((x * t.m41) + (y * t.m42)) + (z * t.m43)) + (d * t.m44));
}

} // namespace ln
