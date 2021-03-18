
#include <LuminoCore/Math/Math.hpp>
#include <LuminoCore/Math/Matrix.hpp>
#include <LuminoCore/Math/Vector2.hpp>
#include <LuminoCore/Math/Vector4.hpp>
#include "Asm.h"

namespace ln {

//==============================================================================
// Vector2

const Vector2 Vector2::Zero = Vector2(0, 0);
const Vector2 Vector2::UnitX = Vector2(1, 0);
const Vector2 Vector2::UnitY = Vector2(0, 1);
const Vector2 Vector2::Ones = Vector2(1, 1);
const Vector2 Vector2::NaN = Vector2(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());

float Vector2::length() const
{
    return Asm::sqrt(x * x + y * y);
}

float Vector2::lengthSquared() const
{
    return (x * x + y * y);
}

void Vector2::normalize()
{
    float t = length();
    if (t == 0.0f)
        return;
    t = 1.0f / t;
    x *= t;
    y *= t;
}

void Vector2::transformCoord(const Matrix& mat)
{
    *this = transformCoord(*this, mat);
}

bool Vector2::isNaNOrInf() const
{
    return Math::isNaNOrInf(x) || Math::isNaNOrInf(y);
}

void Vector2::print(const char* format, FILE* stream) const
{
    if (!format) {
        format = "%f, %f\n";
    }
    if (!stream) {
        stream = stdout;
    }
    fprintf(stream, format, x, y);
}

// static
Vector2 Vector2::normalize(const Vector2& vec)
{
    Vector2 t(vec);
    t.normalize();
    return t;
}

// static
Vector2 Vector2::safeNormalize(const Vector2& vec, const Vector2& alt)
{
	float len = Asm::sqrt(vec.x * vec.x + vec.y * vec.y);
	if (len == 0.0) {
		return alt;
	}
	float t = 1.0f / len;
	return Vector2(
		vec.x * t,
		vec.y * t);
}

float Vector2::distance(const Vector2& vec1, const Vector2& vec2)
{
    float x = vec1.x - vec2.x;
    float y = vec1.y - vec2.y;
    return Asm::sqrt((x * x) + (y * y));
}

// static
float Vector2::dot(const Vector2& vec1, const Vector2& vec2)
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y);
}

// static
Vector2 Vector2::min(const Vector2& vec1, const Vector2& vec2)
{
    return Vector2(
        (vec1.x < vec2.x) ? vec1.x : vec2.x,
        (vec1.y < vec2.y) ? vec1.y : vec2.y);
}

// static
Vector2 Vector2::max(const Vector2& vec1, const Vector2& vec2)
{
    return Vector2(
        (vec1.x > vec2.x) ? vec1.x : vec2.x,
        (vec1.y > vec2.y) ? vec1.y : vec2.y);
}

// static
Vector4 Vector2::transform(const Vector2& vec, const Matrix& mat)
{
    Vector4 t;
    t.x = (vec.x * mat.m[0][0]) + (vec.y * mat.m[1][0]) + mat.m[3][0];
    t.y = (vec.x * mat.m[0][1]) + (vec.y * mat.m[1][1]) + mat.m[3][1];
    t.z = (vec.x * mat.m[0][2]) + (vec.y * mat.m[1][2]) + mat.m[3][2];
    t.w = (vec.x * mat.m[0][3]) + (vec.y * mat.m[1][3]) + mat.m[3][3];
    return t;
}

// static
Vector2 Vector2::transformCoord(const Vector2& vec, const Matrix& mat)
{
    float tx = (vec.x * mat.m[0][0]) + (vec.y * mat.m[1][0]) + mat.m[3][0];
    float ty = (vec.x * mat.m[0][1]) + (vec.y * mat.m[1][1]) + mat.m[3][1];
    //float tz = (vec.x * mat.m[0][3]) + (vec.y * mat.m[1][2]) + mat.m[3][2];
    float tw = 1.0f / ((vec.x * mat.m[0][3]) + (vec.y * mat.m[1][3]) + mat.m[3][3]);
    return Vector2(tx * tw, ty * tw);
}

// static
Vector2 Vector2::lerp(const Vector2& start, const Vector2& end, float t)
{
    return Vector2(
        Math::lerp(start.x, end.x, t),
        Math::lerp(start.y, end.y, t));
}

// static
Vector2 Vector2::hermite(const Vector2& v1, const Vector2& a1, const Vector2& v2, const Vector2& a2, float t)
{
    return Vector2(
        Math::hermite(v1.x, a1.x, v2.x, a2.x, t),
        Math::hermite(v1.y, a1.y, v2.y, a2.y, t));
}

// static
Vector2 Vector2::catmullRom(const Vector2& vec1, const Vector2& vec2, const Vector2& vec3, const Vector2& vec4, float t)
{
    return Vector2(
        Math::catmullRom(vec1.x, vec2.x, vec3.x, vec4.x, t),
        Math::catmullRom(vec1.y, vec2.y, vec3.y, vec4.y, t));
}

bool Vector2::nearEqual(const Vector2& value1, const Vector2& value2)
{
	return Math::nearEqual(value1.x, value2.x) &&
		Math::nearEqual(value1.y, value2.y);
}

} // namespace ln
