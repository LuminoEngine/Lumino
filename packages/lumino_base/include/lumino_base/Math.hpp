#pragma once
#include <cmath>
#include "Types.hpp"

namespace ln {

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
    constexpr Vector3 operator-() const { return {-x, -y, -z}; }
    f32 length() const { return std::sqrt(x * x + y * y + z * z); }
    Vector3 normalized() const {
        f32 len = length();
        return (len > 0) ? Vector3{x / len, y / len, z / len} : Vector3{};
    }

    static constexpr Vector3 cross(const Vector3& a, const Vector3& b) {
        return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    }
    static constexpr f32 dot(const Vector3& a, const Vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static constexpr Vector3 unitX() { return {1, 0, 0}; }
    static constexpr Vector3 unitY() { return {0, 1, 0}; }
    static constexpr Vector3 unitZ() { return {0, 0, 1}; }
    static constexpr Vector3 zero() { return {0, 0, 0}; }
    static constexpr Vector3 one() { return {1, 1, 1}; }
};

struct Vector4 {
    f32 x = 0, y = 0, z = 0, w = 0;

    constexpr Vector4() = default;
    constexpr Vector4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}
    constexpr Vector4(const Vector3& v, f32 w) : x(v.x), y(v.y), z(v.z), w(w) {}
};

struct Color {
    f32 r = 0, g = 0, b = 0, a = 1;

    constexpr Color() = default;
    constexpr Color(f32 r, f32 g, f32 b, f32 a = 1.0f) : r(r), g(g), b(b), a(a) {}

    static constexpr Color white() { return {1, 1, 1, 1}; }
    static constexpr Color black() { return {0, 0, 0, 1}; }
};

struct Quaternion {
    f32 x = 0, y = 0, z = 0, w = 1;

    constexpr Quaternion() = default;
    constexpr Quaternion(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}

    f32 length() const { return std::sqrt(x * x + y * y + z * z + w * w); }
    Quaternion normalized() const {
        f32 len = length();
        return (len > 0) ? Quaternion{x / len, y / len, z / len, w / len} : Quaternion{};
    }

    Quaternion operator*(const Quaternion& q) const {
        return {
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w,
            w * q.w - x * q.x - y * q.y - z * q.z,
        };
    }

    static Quaternion fromAxisAngle(const Vector3& axis, f32 angle);
    static Quaternion fromEuler(f32 pitch, f32 yaw, f32 roll);
    static Quaternion slerp(const Quaternion& a, const Quaternion& b, f32 t);

    static constexpr Quaternion identity() { return {0, 0, 0, 1}; }
};

/** Column-major 4x4 matrix. */
struct Matrix4x4 {
    f32 m[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

    constexpr Matrix4x4() = default;

    static Matrix4x4 perspective(f32 fovY, f32 aspect, f32 nearZ, f32 farZ);
    static Matrix4x4 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ);
    static Matrix4x4 lookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
    static Matrix4x4 translate(const Vector3& t);
    static Matrix4x4 scale(const Vector3& s);
    static Matrix4x4 rotateX(f32 angle);
    static Matrix4x4 rotateY(f32 angle);
    static Matrix4x4 rotateZ(f32 angle);
    static Matrix4x4 rotateAxis(const Vector3& axis, f32 angle);
    static Matrix4x4 fromQuaternion(const Quaternion& q);
    static constexpr Matrix4x4 identity() { return {}; }

    Matrix4x4 transposed() const;
    Matrix4x4 inversed() const;

    Matrix4x4 operator*(const Matrix4x4& rhs) const;
};

} // namespace ln
