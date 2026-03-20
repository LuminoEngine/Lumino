#pragma once
#include <cmath>
#include "types.hpp"

namespace lumino {

struct Vector2 {
    f32 x = 0, y = 0;

    constexpr Vector2() = default;
    constexpr Vector2(f32 x, f32 y) : x(x), y(y) {}

    constexpr Vector2 operator+(const Vector2& v) const { return {x + v.x, y + v.y}; }
    constexpr Vector2 operator-(const Vector2& v) const { return {x - v.x, y - v.y}; }
    constexpr Vector2 operator*(f32 s) const { return {x * s, y * s}; }
    f32 length() const { return std::sqrt(x * x + y * y); }
};

struct Vector3 {
    f32 x = 0, y = 0, z = 0;

    constexpr Vector3() = default;
    constexpr Vector3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}

    constexpr Vector3 operator+(const Vector3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    constexpr Vector3 operator-(const Vector3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    constexpr Vector3 operator*(f32 s) const { return {x * s, y * s, z * s}; }
    f32 length() const { return std::sqrt(x * x + y * y + z * z); }

    static constexpr Vector3 cross(const Vector3& a, const Vector3& b) {
        return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    }
    static constexpr f32 dot(const Vector3& a, const Vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
};

struct Vector4 {
    f32 x = 0, y = 0, z = 0, w = 0;

    constexpr Vector4() = default;
    constexpr Vector4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}
};

struct Color {
    f32 r = 0, g = 0, b = 0, a = 1;

    constexpr Color() = default;
    constexpr Color(f32 r, f32 g, f32 b, f32 a = 1.0f) : r(r), g(g), b(b), a(a) {}

    static constexpr Color white() { return {1, 1, 1, 1}; }
    static constexpr Color black() { return {0, 0, 0, 1}; }
};

/// Column-major 4x4 matrix.
struct Matrix4x4 {
    f32 m[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

    constexpr Matrix4x4() = default;

    static Matrix4x4 perspective(f32 fovY, f32 aspect, f32 nearZ, f32 farZ);
    static Matrix4x4 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ);
    static Matrix4x4 lookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
    static constexpr Matrix4x4 identity() { return {}; }

    Matrix4x4 operator*(const Matrix4x4& rhs) const;
};

} // namespace lumino
