#pragma once
#include "../Types.hpp"

namespace ln {
struct Vector3;
struct Quaternion;

/** 列優先の 4x4 行列。 */
struct Matrix4x4 {
    float m[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

    constexpr Matrix4x4() = default;

    /**
     * 16 個の要素から構築する。
     *
     * 引数は glm::mat4 と同じく列単位で並べます。x0,y0,z0,w0 が第 0 列、x1,y1,z1,w1 が第 1 列
     * ... となり、平行移動成分は最後の x3,y3,z3 に置きます。
     * 行優先の m11,m12,... 表記で書かれた行列をそのまま転記すると転置になるので注意してください。
     */
    Matrix4x4(float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3);

    static Matrix4x4 perspectiveRH(float fovY, float aspect, float nearZ, float farZ);
    static Matrix4x4 ortho(float left, float right, float bottom, float top, float nearZ, float farZ);
    static Matrix4x4 lookAtRH(const Vector3& eye, const Vector3& target, const Vector3& up);
    static Matrix4x4 translate(const Vector3& t);
    static Matrix4x4 scale(const Vector3& s);
    static Matrix4x4 rotateX(float angle);
    static Matrix4x4 rotateY(float angle);
    static Matrix4x4 rotateZ(float angle);
    static Matrix4x4 rotateAxis(const Vector3& axis, float angle);
    static Matrix4x4 fromQuaternion(const Quaternion& q);
    static constexpr Matrix4x4 identity() { return {}; }

    Matrix4x4 transposed() const;
    Matrix4x4 inversed() const;

    /**
     * 点 (w=1) を変換する。列ベクトル規約 out = M * (v, 1)。
     * 透視除算は行わない (アフィン変換用)。
     */
    Vector3 transformCoord(const Vector3& v) const;

    Matrix4x4 operator*(const Matrix4x4& rhs) const;
};

} // namespace ln
