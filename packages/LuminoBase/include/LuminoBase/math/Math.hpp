// 数学関連の型をまとめて取り込むためのヘッダ
#pragma once
#include <cmath>
#include "../Types.hpp"
#include "Matrix4x4.hpp"

namespace ln {

struct Vector2 {
    float x = 0, y = 0;

    constexpr Vector2() = default;
    constexpr Vector2(float x, float y) : x(x), y(y) {}

    constexpr Vector2 operator+(const Vector2& v) const { return {x + v.x, y + v.y}; }
    constexpr Vector2 operator-(const Vector2& v) const { return {x - v.x, y - v.y}; }
    constexpr Vector2 operator*(float s) const { return {x * s, y * s}; }
    float length() const { return std::sqrt(x * x + y * y); }
};

struct Vector3 {
    float x = 0, y = 0, z = 0;

    constexpr Vector3() = default;
    constexpr Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    constexpr Vector3 operator+(const Vector3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    constexpr Vector3 operator-(const Vector3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    constexpr Vector3 operator*(float s) const { return {x * s, y * s, z * s}; }
    constexpr Vector3 operator-() const { return {-x, -y, -z}; }
    float length() const { return std::sqrt(x * x + y * y + z * z); }
    Vector3 normalized() const {
        float len = length();
        return (len > 0) ? Vector3{x / len, y / len, z / len} : Vector3{};
    }
    void normalize() { *this = normalized(); }

    static constexpr Vector3 cross(const Vector3& a, const Vector3& b) {
        return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    }
    static constexpr float dot(const Vector3& a, const Vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static constexpr Vector3 unitX() { return {1, 0, 0}; }
    static constexpr Vector3 unitY() { return {0, 1, 0}; }
    static constexpr Vector3 unitZ() { return {0, 0, 1}; }
    static constexpr Vector3 zero() { return {0, 0, 0}; }
    static constexpr Vector3 one() { return {1, 1, 1}; }
};

struct Vector4 {
    float x = 0, y = 0, z = 0, w = 0;

    constexpr Vector4() = default;
    constexpr Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    constexpr Vector4(const Vector3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
};

struct Color {
    float r = 0, g = 0, b = 0, a = 1;

    constexpr Color() = default;
    constexpr Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}

    static constexpr Color white() { return {1, 1, 1, 1}; }
    static constexpr Color black() { return {0, 0, 0, 1}; }
};

struct Quaternion {
    float x = 0, y = 0, z = 0, w = 1;

    constexpr Quaternion() = default;
    constexpr Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    float length() const { return std::sqrt(x * x + y * y + z * z + w * w); }
    Quaternion normalized() const {
        float len = length();
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

    static Quaternion fromAxisAngle(const Vector3& axis, float angle);
    static Quaternion fromEuler(float pitch, float yaw, float roll);
    static Quaternion slerp(const Quaternion& a, const Quaternion& b, float t);

    static constexpr Quaternion identity() { return {0, 0, 0, 1}; }
};

} // namespace ln
