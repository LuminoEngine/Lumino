
#include <LuminoCore/Math/Math.hpp>
#include <LuminoCore/Math/Vector4.hpp>
#include <LuminoCore/Math/Matrix.hpp>
#include "Asm.h"

namespace ln {

//==============================================================================
// Vector4

const Vector4 Vector4::Zero(0, 0, 0, 0);

float Vector4::length() const
{
    return Asm::sqrt(x * x + y * y + z * z + w * w);
}

float Vector4::lengthSquared() const
{
    return x * x + y * y + z * z + w * w;
}

void Vector4::clamp(float min_xyzw, float max_xyzw)
{
    x = Math::clamp(x, min_xyzw, max_xyzw);
    y = Math::clamp(y, min_xyzw, max_xyzw);
    z = Math::clamp(z, min_xyzw, max_xyzw);
    w = Math::clamp(w, min_xyzw, max_xyzw);
}

void Vector4::clamp(const Vector4& minVec, const Vector4& maxVec)
{
    x = Math::clamp(x, minVec.x, maxVec.x);
    y = Math::clamp(y, minVec.y, maxVec.y);
    z = Math::clamp(z, minVec.z, maxVec.z);
    w = Math::clamp(w, minVec.w, maxVec.w);
}

bool Vector4::isNaNOrInf() const
{
    return Math::isNaNOrInf(x) || Math::isNaNOrInf(y) || Math::isNaNOrInf(z) || Math::isNaNOrInf(w);
}
//
////void Vector4::print(const char* format, FILE* stream) const
//{
//	if (!format) {
//		format = "%f, %f, %f, %f\n";
//	}
//	if (!stream) {
//		stream = stdout;
//	}
//	fprintf(stream, format, x, y, z, w);
//}

// static
Vector4 Vector4::normalize(const Vector4& vec)
{
    float t = 1.0f / Asm::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
    return Vector4(
        vec.x * t,
        vec.y * t,
        vec.z * t,
        vec.w * t);
}

// static
float Vector4::dot(const Vector4& vec1, const Vector4& vec2)
{
    return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w);
}

// static
Vector4 Vector4::min(const Vector4& vec1, const Vector4& vec2)
{
    return Vector4(
        (vec1.x < vec2.x) ? vec1.x : vec2.x,
        (vec1.y < vec2.y) ? vec1.y : vec2.y,
        (vec1.z < vec2.z) ? vec1.z : vec2.z,
        (vec1.w < vec2.w) ? vec1.w : vec2.w);
}

// static
Vector4 Vector4::max(const Vector4& vec1, const Vector4& vec2)
{
    return Vector4(
        (vec1.x > vec2.x) ? vec1.x : vec2.x,
        (vec1.y > vec2.y) ? vec1.y : vec2.y,
        (vec1.z > vec2.z) ? vec1.z : vec2.z,
        (vec1.w > vec2.w) ? vec1.w : vec2.w);
}

// static
Vector4 Vector4::transform(const Vector4& vec, const Matrix& mat)
{
    return Vector4(
        (vec.x * mat.m11) + (vec.y * mat.m21) + (vec.z * mat.m31) + (vec.w * mat.m41),
        (vec.x * mat.m12) + (vec.y * mat.m22) + (vec.z * mat.m32) + (vec.w * mat.m42),
        (vec.x * mat.m13) + (vec.y * mat.m23) + (vec.z * mat.m33) + (vec.w * mat.m43),
        (vec.x * mat.m14) + (vec.y * mat.m24) + (vec.z * mat.m34) + (vec.w * mat.m44));
}

// static
Vector4 Vector4::lerp(const Vector4& start, const Vector4& end, float t)
{
    return Vector4(
        Math::lerp(start.x, end.x, t),
        Math::lerp(start.y, end.y, t),
        Math::lerp(start.z, end.z, t),
        Math::lerp(start.w, end.w, t));
}

// static
Vector4 Vector4::hermite(const Vector4& v1, const Vector4& a1, const Vector4& v2, const Vector4& a2, float t)
{
    return Vector4(
        Math::hermite(v1.x, a1.x, v2.x, a2.x, t),
        Math::hermite(v1.y, a1.y, v2.y, a2.y, t),
        Math::hermite(v1.z, a1.z, v2.z, a2.z, t),
        Math::hermite(v1.w, a1.w, v2.w, a2.w, t));
}

// static
Vector4 Vector4::catmullRom(const Vector4& vec1, const Vector4& vec2, const Vector4& vec3, const Vector4& vec4, float t)
{
    return Vector4(
        Math::catmullRom(vec1.x, vec2.x, vec3.x, vec4.x, t),
        Math::catmullRom(vec1.y, vec2.y, vec3.y, vec4.y, t),
        Math::catmullRom(vec1.z, vec2.z, vec3.z, vec4.z, t),
        Math::catmullRom(vec1.w, vec2.w, vec3.w, vec4.w, t));
}

bool Vector4::nearEqual(const Vector4& value1, const Vector4& value2)
{
    return
        Math::nearEqual(value1.x, value2.x) &&
        Math::nearEqual(value1.y, value2.y) &&
        Math::nearEqual(value1.z, value2.z) &&
        Math::nearEqual(value1.w, value2.w);
}

} // namespace ln
