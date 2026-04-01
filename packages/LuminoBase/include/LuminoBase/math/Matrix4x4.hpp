#pragma once
#include "../Types.hpp"

namespace ln {
struct Vector3;
struct Quaternion;

/** Column-major 4x4 matrix. */
struct Matrix4x4 {
    f32 m[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

    //Matrix4x4();
    constexpr Matrix4x4() = default;
    Matrix4x4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
    

    static Matrix4x4 perspectiveRH(f32 fovY, f32 aspect, f32 nearZ, f32 farZ);
    static Matrix4x4 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ);
    static Matrix4x4 lookAtRH(const Vector3& eye, const Vector3& target, const Vector3& up);
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
