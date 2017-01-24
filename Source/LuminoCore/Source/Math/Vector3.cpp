
#include "../../include/Lumino/Math/MathUtils.h"
#include "../../include/Lumino/Math/Vector2.h"
#include "../../include/Lumino/Math/Vector3.h"
#include "../../include/Lumino/Math/Vector4.h"
#include "../../include/Lumino/Math/Quaternion.h"
#include "../../include/Lumino/Math/Matrix.h"
#include "Asm.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Vector3
//==============================================================================
	
const Vector3   Vector3::Zero	= Vector3(0, 0, 0);
const Vector3   Vector3::UnitX	= Vector3(1, 0, 0);
const Vector3   Vector3::UnitY	= Vector3(0, 1, 0);
const Vector3   Vector3::UnitZ	= Vector3(0, 0, 1);
const Vector3   Vector3::Ones	= Vector3(1, 1, 1);

//------------------------------------------------------------------------------
Vector3::Vector3(const Vector2& vec, float z_)
{
	x = vec.x;
	y = vec.y;
	z = z_;
}

//------------------------------------------------------------------------------
float Vector3::GetLength() const
{
	return Asm::sqrt(x * x + y * y + z * z);
}

//------------------------------------------------------------------------------
void Vector3::Normalize()
{
	float t = 1.0f / Asm::sqrt(x * x + y * y + z * z);
	x *= t;
	y *= t;
	z *= t;
}

//------------------------------------------------------------------------------
void Vector3::Clamp(const Vector3& minVec, const Vector3& maxVec)
{
	x = Math::Clamp(x, minVec.x, maxVec.x);
	y = Math::Clamp(y, minVec.y, maxVec.y);
	z = Math::Clamp(z, minVec.z, maxVec.z);
}

//------------------------------------------------------------------------------
void Vector3::TransformCoord(const Matrix& mat)
{
	float tx = x;
	float ty = y;
	float tz = z;
	float w = 1.0f / (tx * mat.m[0][3] + ty * mat.m[1][3] + tz * mat.m[2][3] + mat.m[3][3]);
	x = (tx * mat.m[0][0] + ty * mat.m[1][0] + tz * mat.m[2][0] + mat.m[3][0]) * w;
	y = (tx * mat.m[0][1] + ty * mat.m[1][1] + tz * mat.m[2][1] + mat.m[3][1]) * w;
	z = (tx * mat.m[0][2] + ty * mat.m[1][2] + tz * mat.m[2][2] + mat.m[3][2]) * w;
}

//------------------------------------------------------------------------------
bool Vector3::IsNaNOrInf() const
{
	return Math::IsNaNOrInf(x) || Math::IsNaNOrInf(y) || Math::IsNaNOrInf(z);
}

//------------------------------------------------------------------------------
void Vector3::Print(const char* format, FILE* stream) const
{
	if (!format) {
		format = "%f, %f, %f\n";
	}
	if (!stream) {
		stream = stdout;
	}
	fprintf(stream, format, x, y, z);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector3 Vector3::Normalize(float x, float y, float z)
{
	float t = 1.0f / Asm::sqrt(x * x + y * y + z * z);
	return Vector3(
		x * t,
		y * t,
		z * t);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector3 Vector3::Normalize(const Vector3& vec)
{
	float t = 1.0f / Asm::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return Vector3(
		vec.x * t,
		vec.y * t,
		vec.z * t);
}

//------------------------------------------------------------------------------
Vector3 Vector3::SafeNormalize(const Vector3& vec)
{
	float len = Asm::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (len == 0.0)
	{
		return vec;
	}
	float t = 1.0f / len;
	return Vector3(
		vec.x * t,
		vec.y * t,
		vec.z * t);
}

//------------------------------------------------------------------------------
Vector3 Vector3::SafeNormalize(const Vector3& vec, const Vector3& alt)
{
	float len = Asm::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (len == 0.0)
	{
		return alt;
	}
	float t = 1.0f / len;
	return Vector3(
		vec.x * t,
		vec.y * t,
		vec.z * t);
}

//------------------------------------------------------------------------------
float Vector3::Distance(const Vector3& vec1, const Vector3& vec2)
{
	float x = vec1.x - vec2.x;
	float y = vec1.y - vec2.y;
	float z = vec1.z - vec2.z;
	return Asm::sqrt((x * x) + (y * y) + (z * z));
}

//------------------------------------------------------------------------------
float Vector3::DistanceSquared(const Vector3& vec1, const Vector3& vec2)
{
	float x = vec1.x - vec2.x;
	float y = vec1.y - vec2.y;
	float z = vec1.z - vec2.z;
	return (x * x) + (y * y) + (z * z);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
float Vector3::Dot(const Vector3& vec1, const Vector3& vec2)
{
	return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector3 Vector3::Cross(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3(
		(vec1.y * vec2.z) - (vec1.z * vec2.y),
		(vec1.z * vec2.x) - (vec1.x * vec2.z),
		(vec1.x * vec2.y) - (vec1.y * vec2.x));
}

//------------------------------------------------------------------------------
Vector3 Vector3::Min(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3(
		(vec1.x < vec2.x) ? vec1.x : vec2.x,
		(vec1.y < vec2.y) ? vec1.y : vec2.y,
		(vec1.z < vec2.z) ? vec1.z : vec2.z);
}

//------------------------------------------------------------------------------
Vector3 Vector3::Min(const Vector3* vectors, int count)
{
	if (count <= 0) return Vector3::Zero;
	Vector3 out = vectors[0];
	for (int i = 1; i < count; ++i)
	{
		out = Min(out, vectors[i]);
	}
	return out;
}

//------------------------------------------------------------------------------
Vector3 Vector3::Max(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3(
		(vec1.x > vec2.x) ? vec1.x : vec2.x,
		(vec1.y > vec2.y) ? vec1.y : vec2.y,
		(vec1.z > vec2.z) ? vec1.z : vec2.z);
}

//------------------------------------------------------------------------------
Vector3 Vector3::Max(const Vector3* vectors, int count)
{
	if (count <= 0) return Vector3::Zero;
	Vector3 out = vectors[0];
	for (int i = 1; i < count; ++i)
	{
		out = Max(out, vectors[i]);
	}
	return out;
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector3 Vector3::Reflect(const Vector3& vec, const Vector3& normal)
{
	float t = -( normal.x * vec.x + normal.y * vec.y + normal.z * vec.z ) / ( normal.x * normal.x + normal.y * normal.y + normal.z * normal.z );
	return Vector3(
		vec.x + t * normal.x * 2.0f,
		vec.y + t * normal.y * 2.0f,
		vec.z + t * normal.z * 2.0f );
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector3 Vector3::Slide(const Vector3& vec, const Vector3& normal)
{
	float t = -(normal.x * vec.x + normal.y * vec.y + normal.z * vec.z) / (normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
	return Vector3(
		vec.x + t * normal.x,
		vec.y + t * normal.y,
		vec.z + t * normal.z );
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector3 Vector3::Transform(const Vector3& vec, const Quaternion& qua)
{
	float x = qua.x + qua.x;
	float y = qua.y + qua.y;
	float z = qua.z + qua.z;
	float wx = qua.w * x;
	float wy = qua.w * y;
	float wz = qua.w * z;
	float xx = qua.x * x;
	float xy = qua.x * y;
	float xz = qua.x * z;
	float yy = qua.y * y;
	float yz = qua.y * z;
	float zz = qua.z * z;

	return Vector3(
		((vec.x * ((1.0f - yy) - zz)) + (vec.y * (xy - wz))) + (vec.z * (xz + wy)),
		((vec.x * (xy + wz)) + (vec.y * ((1.0f - xx) - zz))) + (vec.z * (yz - wx)),
		((vec.x * (xz - wy)) + (vec.y * (yz + wx))) + (vec.z * ((1.0f - xx) - yy)));
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector4 Vector3::Transform(const Vector3& vec, const Matrix& mat)
{
	return Vector4(
		(((vec.x * mat.m11) + (vec.y * mat.m21)) + (vec.z * mat.m31)) + mat.m41,
		(((vec.x * mat.m12) + (vec.y * mat.m22)) + (vec.z * mat.m32)) + mat.m42,
		(((vec.x * mat.m13) + (vec.y * mat.m23)) + (vec.z * mat.m33)) + mat.m43,
		(((vec.x * mat.m14) + (vec.y * mat.m24)) + (vec.z * mat.m34)) + mat.m44);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector3 Vector3::TransformCoord(const Vector3& vec, const Matrix& mat)
{
	float w = 1.0f / ((((vec.x * mat.m14) + (vec.y * mat.m24)) + (vec.z * mat.m34)) + mat.m44);
	return Vector3(
		((vec.x * mat.m11) + (vec.y * mat.m21) + (vec.z * mat.m31) + mat.m41) * w,
		((vec.x * mat.m12) + (vec.y * mat.m22) + (vec.z * mat.m32) + mat.m42) * w,
		((vec.x * mat.m13) + (vec.y * mat.m23) + (vec.z * mat.m33) + mat.m43) * w);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float t)
{
	return Vector3(
		Math::Lerp(start.x, end.x, t),
		Math::Lerp(start.y, end.y, t),
		Math::Lerp(start.z, end.z, t));
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector3 Vector3::Hermite(const Vector3& v1, const Vector3& a1, const Vector3& v2, const Vector3& a2, float t)
{
	return Vector3(
		Math::Hermite(v1.x, a1.x, v2.x, a2.x, t),
		Math::Hermite(v1.y, a1.y, v2.y, a2.y, t),
		Math::Hermite(v1.z, a1.z, v2.z, a2.z, t));
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector3 Vector3::CatmullRom(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3, const Vector3& vec4, float t)
{
	return Vector3(
		Math::CatmullRom(vec1.x, vec2.x, vec3.x, vec4.x, t),
		Math::CatmullRom(vec1.y, vec2.y, vec3.y, vec4.y, t),
		Math::CatmullRom(vec1.z, vec2.z, vec3.z, vec4.z, t));
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector3 Vector3::Project(const Vector3& point, const Matrix& worldViewProj, float x, float y, float width, float height, float minZ, float maxZ)
{
	Vector3 v = Vector3::TransformCoord(point, worldViewProj);
	return Vector3(
		((1.0f + v.x) * 0.5f * width) + x, 
		((1.0f - v.y) * 0.5f * height) + y, 
		(v.z * (maxZ - minZ)) + minZ);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector3 Vector3::Unproject(const Vector3& point, const Matrix& worldViewProj, float x, float y, float width, float height, float minZ, float maxZ)
{
	Vector3 v;
	Matrix inv = Matrix::MakeInverse(worldViewProj);

	v.x = (((point.x - x) / width) * 2.0f) - 1.0f;
	v.y = -((((point.y - y) / height) * 2.0f) - 1.0f);
	v.z = (point.z - minZ) / (maxZ - minZ);

	return Vector3::TransformCoord(v, inv);
}

LN_NAMESPACE_END
