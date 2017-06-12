
#include "../../include/Lumino/Math/MathUtils.h"
#include "../../include/Lumino/Math/Vector2.h"
#include "../../include/Lumino/Math/Vector3.h"
#include "../../include/Lumino/Math/Vector4.h"
#include "../../include/Lumino/Math/Matrix.h"
#include "Asm.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Vector4
//==============================================================================

const Vector4 Vector4::Zero(0, 0, 0, 0);

//------------------------------------------------------------------------------
Vector4::Vector4(const Vector2& vec, float z_, float w_)
{
	x = vec.x;
	y = vec.y;
	z = z_;
	w = w_;
}

//------------------------------------------------------------------------------
Vector4::Vector4(const Vector3& vec, float w_)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = w_;
}

//------------------------------------------------------------------------------
float Vector4::getLength() const
{
	return Asm::sqrt(x * x + y * y + z * z + w * w);
}

//------------------------------------------------------------------------------
float Vector4::GetLengthSquared() const
{
	return x * x + y * y + z * z + w * w;
}

//------------------------------------------------------------------------------
void Vector4::Clamp(float min_xyzw, float max_xyzw)
{
	x = Math::Clamp(x, min_xyzw, max_xyzw);
	y = Math::Clamp(y, min_xyzw, max_xyzw);
	z = Math::Clamp(z, min_xyzw, max_xyzw);
	w = Math::Clamp(w, min_xyzw, max_xyzw);
}

//------------------------------------------------------------------------------
void Vector4::Clamp(const Vector4& minVec, const Vector4& maxVec)
{
	x = Math::Clamp(x, minVec.x, maxVec.x);
	y = Math::Clamp(y, minVec.y, maxVec.y);
	z = Math::Clamp(z, minVec.z, maxVec.z);
	w = Math::Clamp(w, minVec.w, maxVec.w);
}

//------------------------------------------------------------------------------
bool Vector4::IsNaNOrInf() const
{
	return Math::IsNaNOrInf(x) || Math::IsNaNOrInf(y) || Math::IsNaNOrInf(z) || Math::IsNaNOrInf(w);
}

//------------------------------------------------------------------------------
void Vector4::Print(const char* format, FILE* stream) const
{
	if (!format) {
		format = "%f, %f, %f, %f\n";
	}
	if (!stream) {
		stream = stdout;
	}
	fprintf(stream, format, x, y, z, w);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector4 Vector4::Normalize(const Vector4& vec)
{
	float t = 1.0f / Asm::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
	return Vector4(
		vec.x * t,
		vec.y * t,
		vec.z * t,
		vec.w * t);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
float Vector4::Dot(const Vector4& vec1, const Vector4& vec2)
{
	return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector4 Vector4::Min(const Vector4& vec1, const Vector4& vec2)
{
	return Vector4(
		(vec1.x < vec2.x) ? vec1.x : vec2.x,
		(vec1.y < vec2.y) ? vec1.y : vec2.y,
		(vec1.z < vec2.z) ? vec1.z : vec2.z,
		(vec1.w < vec2.w) ? vec1.w : vec2.w);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector4 Vector4::Max(const Vector4& vec1, const Vector4& vec2)
{
	return Vector4(
		(vec1.x > vec2.x) ? vec1.x : vec2.x,
		(vec1.y > vec2.y) ? vec1.y : vec2.y,
		(vec1.z > vec2.z) ? vec1.z : vec2.z,
		(vec1.w > vec2.w) ? vec1.w : vec2.w);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector4 Vector4::Transform(const Vector4& vec, const Matrix& mat)
{
	return Vector4(
		(vec.x * mat.m11) + (vec.y * mat.m21) + (vec.z * mat.m31) + (vec.w * mat.m41),
		(vec.x * mat.m12) + (vec.y * mat.m22) + (vec.z * mat.m32) + (vec.w * mat.m42),
		(vec.x * mat.m13) + (vec.y * mat.m23) + (vec.z * mat.m33) + (vec.w * mat.m43),
		(vec.x * mat.m14) + (vec.y * mat.m24) + (vec.z * mat.m34) + (vec.w * mat.m44));
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector4 Vector4::Lerp(const Vector4& start, const Vector4& end, float t)
{
	return Vector4(
		Math::Lerp(start.x, end.x, t),
		Math::Lerp(start.y, end.y, t),
		Math::Lerp(start.z, end.z, t),
		Math::Lerp(start.w, end.w, t));
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector4 Vector4::Hermite(const Vector4& v1, const Vector4& a1, const Vector4& v2, const Vector4& a2, float t)
{
	return Vector4(
		Math::Hermite(v1.x, a1.x, v2.x, a2.x, t),
		Math::Hermite(v1.y, a1.y, v2.y, a2.y, t),
		Math::Hermite(v1.z, a1.z, v2.z, a2.z, t),
		Math::Hermite(v1.w, a1.w, v2.w, a2.w, t));
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector4 Vector4::CatmullRom(const Vector4& vec1, const Vector4& vec2, const Vector4& vec3, const Vector4& vec4, float t)
{
	return Vector4(
		Math::CatmullRom(vec1.x, vec2.x, vec3.x, vec4.x, t),
		Math::CatmullRom(vec1.y, vec2.y, vec3.y, vec4.y, t),
		Math::CatmullRom(vec1.z, vec2.z, vec3.z, vec4.z, t),
		Math::CatmullRom(vec1.w, vec2.w, vec3.w, vec4.w, t));
}

LN_NAMESPACE_END
