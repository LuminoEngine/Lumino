
#include "../../include/Lumino/Math/MathUtils.h"
#include "../../include/Lumino/Math/Matrix.h"
#include "../../include/Lumino/Math/Vector2.h"
#include "../../include/Lumino/Math/Vector4.h"
#include "Asm.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Vector2
//==============================================================================

const Vector2   Vector2::Zero	= Vector2(0, 0);
const Vector2   Vector2::UnitX	= Vector2(1, 0);
const Vector2   Vector2::UnitY	= Vector2(0, 1);
const Vector2   Vector2::Ones	= Vector2(1, 1);

//------------------------------------------------------------------------------
float Vector2::GetLength() const
{
    return Asm::sqrt( x * x + y * y );
}

//------------------------------------------------------------------------------
float Vector2::GetLengthSquared() const
{
    return ( x * x + y * y );
}

//------------------------------------------------------------------------------
void Vector2::Normalize()
{
	float t = GetLength();
	if (t == 0.0f)
		return;
	t = 1.0f / t;
	x *= t;
	y *= t;
}

//------------------------------------------------------------------------------
void Vector2::TransformCoord(const Matrix& mat)
{
	*this = TransformCoord(*this, mat);
}

//------------------------------------------------------------------------------
bool Vector2::IsNaNOrInf() const
{
	return Math::IsNaNOrInf(x) || Math::IsNaNOrInf(y);
}

//------------------------------------------------------------------------------
void Vector2::Print(const char* format, FILE* stream) const
{
	if (!format) {
		format = "%f, %f\n";
	}
	if (!stream) {
		stream = stdout;
	}
	fprintf(stream, format, x, y);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector2 Vector2::Normalize(const Vector2& vec)
{
	Vector2 t(vec);
	t.Normalize();
	return t;
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
float Vector2::Dot(const Vector2& vec1, const Vector2& vec2)
{
	return (vec1.x * vec2.x) + (vec1.y * vec2.y);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector2 Vector2::Min(const Vector2& vec1, const Vector2& vec2)
{
	return Vector2(
		(vec1.x < vec2.x) ? vec1.x : vec2.x,
		(vec1.y < vec2.y) ? vec1.y : vec2.y);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector2 Vector2::Max(const Vector2& vec1, const Vector2& vec2)
{
	return Vector2(
		(vec1.x > vec2.x) ? vec1.x : vec2.x,
		(vec1.y > vec2.y) ? vec1.y : vec2.y);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector4 Vector2::Transform(const Vector2& vec, const Matrix& mat)
{
	Vector4 t;
	t.x = (vec.x * mat.m[0][0]) + (vec.y * mat.m[1][0]) + mat.m[3][0];
	t.y = (vec.x * mat.m[0][1]) + (vec.y * mat.m[1][1]) + mat.m[3][1];
	t.z = (vec.x * mat.m[0][2]) + (vec.y * mat.m[1][2]) + mat.m[3][2];
	t.w = (vec.x * mat.m[0][3]) + (vec.y * mat.m[1][3]) + mat.m[3][3];
	return t;
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector2 Vector2::TransformCoord(const Vector2& vec, const Matrix& mat)
{
	float tx = (vec.x * mat.m[0][0]) + (vec.y * mat.m[1][0]) + mat.m[3][0];
	float ty = (vec.x * mat.m[0][1]) + (vec.y * mat.m[1][1]) + mat.m[3][1];
	//float tz = (vec.x * mat.m[0][3]) + (vec.y * mat.m[1][2]) + mat.m[3][2];
	float tw = 1.0f / ((vec.x * mat.m[0][3]) + (vec.y * mat.m[1][3]) + mat.m[3][3]);
	return Vector2(tx * tw, ty * tw);
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector2 Vector2::Lerp(const Vector2& start, const Vector2& end, float t)
{
	return Vector2(
		Math::Lerp(start.x, end.x, t),
		Math::Lerp(start.y, end.y, t));
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector2 Vector2::Hermite(const Vector2& v1, const Vector2& a1, const Vector2& v2, const Vector2& a2, float t)
{
	return Vector2(
		Math::Hermite(v1.x, a1.x, v2.x, a2.x, t),
		Math::Hermite(v1.y, a1.y, v2.y, a2.y, t));
}

//------------------------------------------------------------------------------
// static
//------------------------------------------------------------------------------
Vector2 Vector2::CatmullRom(const Vector2& vec1, const Vector2& vec2, const Vector2& vec3, const Vector2& vec4, float t)
{
	return Vector2(
		Math::CatmullRom(vec1.x, vec2.x, vec3.x, vec4.x, t),
		Math::CatmullRom(vec1.y, vec2.y, vec3.y, vec4.y, t));
}

LN_NAMESPACE_END
