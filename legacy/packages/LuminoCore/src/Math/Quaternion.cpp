
#include <assert.h>
#include <LuminoCore/Math/Math.hpp>
#include <LuminoCore/Math/Vector3.hpp>
#include <LuminoCore/Math/Matrix.hpp>
#include <LuminoCore/Math/Quaternion.hpp>
#include "Asm.h"

namespace ln {

//==============================================================================
// Quaternion

const Quaternion Quaternion::Identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

Quaternion::Quaternion(const Vector3& axis, float r)
{
    float tx = axis.x;
    float ty = axis.y;
    float tz = axis.z;
    float len = tx * tx + ty * ty + tz * tz;

    // 軸が正規化されていなければ正規化
    if (len != 1.0f) {
        len = Asm::sqrt(len);
        tx /= len;
        ty /= len;
        tz /= len;
    }

    float s, c;
    Asm::sincos(r * 0.5f, &s, &c);
    x = s * tx;
    y = s * ty;
    z = s * tz;
    w = c;
}

float Quaternion::length() const
{
    return Asm::sqrt((x * x) + (y * y) + (z * z) + (w * w));
}

float Quaternion::lengthSquared() const
{
    return (x * x) + (y * y) + (z * z) + (w * w);
}

void Quaternion::normalize()
{
    float t = 1.0f / length();
    x *= t;
    y *= t;
    z *= t;
    w *= t;
}

void Quaternion::multiply(const Quaternion& qua)
{
    float lx = x;
    float ly = y;
    float lz = z;
    float lw = w;
    x = (qua.x * lw + lx * qua.w + qua.y * lz) - (qua.z * ly);
    y = (qua.y * lw + ly * qua.w + qua.z * lx) - (qua.x * lz);
    z = (qua.z * lw + lz * qua.w + qua.x * ly) - (qua.y * lx);
    w = (qua.w * lw) - (qua.x * lx + qua.y * ly + qua.z * lz);
}

void Quaternion::rotateX(float angle)
{
    float s, c;
    Asm::sincos(angle * 0.5f, &s, &c);

    float lx = x;
    float ly = y;
    float lz = z;
    float lw = w;
    x = (s * lw + lx * c);
    y = (ly * c) - (s * lz);
    z = (lz * c + s * ly);
    w = (c * lw) - (s * lx);
}

void Quaternion::rotateY(float angle)
{
    float s, c;
    Asm::sincos(angle * 0.5f, &s, &c);

    float lx = x;
    float ly = y;
    float lz = z;
    float lw = w;
    x = (lx * c + s * lz);
    y = (s * lw + ly * c);
    z = (lz * c) - (s * lx);
    w = (c * lw) - (s * ly);
}

void Quaternion::rotateZ(float angle)
{
    float s, c;
    Asm::sincos(angle * 0.5f, &s, &c);

    float lx = x;
    float ly = y;
    float lz = z;
    float lw = w;
    x = (lx * c) - (s * ly);
    y = (ly * c + s * lx);
    z = (s * lw + lz * c);
    w = (c * lw) - (s * lz);
}

void Quaternion::rotateAxis(const Vector3& axis, float r)
{
    // TODO: optimize
    multiply(makeFromRotationAxis(axis, r));
}

Vector3 Quaternion::toEulerAngles(RotationOrder order, bool* locked) const
{
    // クォータニオン→行列
    Matrix rot = Matrix::makeRotationQuaternion(Quaternion::normalize(*this));
    return rot.toEulerAngles(order, locked);
}

void Quaternion::toAxisAngle(Vector3* axis, float* angle) const
{
    Quaternion q = Quaternion::normalize(*this);
    if (axis) {
        axis->x = q.x;
        axis->y = q.y;
        axis->z = q.z;
        axis->mutatingNormalize();
    }
    if (angle) {
        *angle = 2.0f * acosf(q.w);
    }
}

bool Quaternion::isNaNOrInf() const
{
    return Math::isNaNOrInf(x) || Math::isNaNOrInf(y) || Math::isNaNOrInf(z) || Math::isNaNOrInf(w);
}

// static
Quaternion Quaternion::normalize(const Quaternion& qua)
{
    float length = 1.0f / qua.length();
    return Quaternion(
        qua.x * length,
        qua.y * length,
        qua.z * length,
        qua.w * length);
}

// static
Quaternion Quaternion::conjugate(const Quaternion& qua)
{
    return Quaternion(
        -qua.x,
        -qua.y,
        -qua.z,
        qua.w);
}

// static
float Quaternion::dot(const Quaternion& qua1, const Quaternion& qua2)
{
    return (qua1.x * qua2.x) + (qua1.y * qua2.y) + (qua1.z * qua2.z) + (qua1.w * qua2.w);
}

// static
Quaternion Quaternion::multiply(const Quaternion& qua1, const Quaternion& qua2)
{
    return Quaternion(
        (qua2.x * qua1.w + qua1.x * qua2.w + qua2.y * qua1.z) - (qua2.z * qua1.y),
        (qua2.y * qua1.w + qua1.y * qua2.w + qua2.z * qua1.x) - (qua2.x * qua1.z),
        (qua2.z * qua1.w + qua1.z * qua2.w + qua2.x * qua1.y) - (qua2.y * qua1.x),
        (qua2.w * qua1.w) - (qua2.x * qua1.x + qua2.y * qua1.y + qua2.z * qua1.z));
}

// static
Quaternion Quaternion::makeInverse(const Quaternion& qua)
{
    float t = 1.0f / ((qua.x * qua.x) + (qua.y * qua.y) + (qua.z * qua.z) + (qua.w * qua.w));
    return Quaternion(
        -qua.x * t,
        -qua.y * t,
        -qua.z * t,
        qua.w * t);
}

// static
Quaternion Quaternion::makeFromRotationAxis(const Vector3& axis, float r)
{
    float tx = axis.x;
    float ty = axis.y;
    float tz = axis.z;
    float len = tx * tx + ty * ty + tz * tz;

    // 軸が正規化されていなければ正規化
    if (len != 1.0f) {
        len = Asm::sqrt(len);
        tx /= len;
        ty /= len;
        tz /= len;
    }

    float s, c;
    Asm::sincos(r * 0.5f, &s, &c);
    return Quaternion(
        s * tx,
        s * ty,
        s * tz,
        c);
}

// static
Quaternion Quaternion::makeFromRotationMatrix(const Matrix& mat)
{
    float t = mat.m11 + mat.m22 + mat.m33;

    if (t > 0.0f) {
        float sq = Asm::sqrt(t + 1.0f);

        float w = sq * 0.5f;
        sq = 0.5f / sq;

        return Quaternion(
            (mat.m23 - mat.m32) * sq,
            (mat.m31 - mat.m13) * sq,
            (mat.m12 - mat.m21) * sq,
            w);
    } else if ((mat.m11 >= mat.m22) && (mat.m11 >= mat.m33)) {
        float sq = Asm::sqrt(1.0f + mat.m11 - mat.m22 - mat.m33);
        float half = 0.5f / sq;

        return Quaternion(
            0.5f * sq,
            (mat.m12 + mat.m21) * half,
            (mat.m13 + mat.m31) * half,
            (mat.m23 - mat.m32) * half);
    } else if (mat.m22 > mat.m33) {
        float sq = Asm::sqrt(1.0f + mat.m22 - mat.m11 - mat.m33);
        float half = 0.5f / sq;

        return Quaternion(
            (mat.m21 + mat.m12) * half,
            0.5f * sq,
            (mat.m32 + mat.m23) * half,
            (mat.m31 - mat.m13) * half);
    } else {
        float sq = Asm::sqrt(1.0f + mat.m33 - mat.m11 - mat.m22);
        float half = 0.5f / sq;

        return Quaternion(
            (mat.m31 + mat.m13) * half,
            (mat.m32 + mat.m23) * half,
            0.5f * sq,
            (mat.m12 - mat.m21) * half);
    }
}

// static
Quaternion Quaternion::makeFromYawPitchRoll(float yaw, float pitch, float roll)
{
    float halfRoll = roll * 0.5f;
    float sinRoll, cosRoll;
    Asm::sincos(halfRoll, &sinRoll, &cosRoll);

    float halfPitch = pitch * 0.5f;
    float sinPitch, cosPitch;
    Asm::sincos(halfPitch, &sinPitch, &cosPitch);

    float halfYaw = yaw * 0.5f;
    float sinYaw, cosYaw;
    Asm::sincos(halfYaw, &sinYaw, &cosYaw);

    return Quaternion(
        (cosYaw * sinPitch * cosRoll) + (sinYaw * cosPitch * sinRoll),
        (sinYaw * cosPitch * cosRoll) - (cosYaw * sinPitch * sinRoll),
        (cosYaw * cosPitch * sinRoll) - (sinYaw * sinPitch * cosRoll),
        (cosYaw * cosPitch * cosRoll) + (sinYaw * sinPitch * sinRoll));
}

// static
Quaternion Quaternion::makeFromEulerAngles(const Vector3& angles, RotationOrder order)
{
    Quaternion q;
    switch (order) {
        case RotationOrder::XYZ:
            q.rotateX(angles.x);
            q.rotateY(angles.y);
            q.rotateZ(angles.z);
            break;
        case RotationOrder::YZX:
            q.rotateY(angles.y);
            q.rotateZ(angles.z);
            q.rotateX(angles.x);
            break;
        case RotationOrder::ZXY:
            q.rotateZ(angles.z);
            q.rotateX(angles.x);
            q.rotateY(angles.y);
            break;
        default:
            assert(0);
            break;
    }
    return q;
}

// static
Quaternion Quaternion::slerp(const Quaternion& qua1, const Quaternion& qua2, float t)
{
    float opposite;
    float inverse;
    float dot = (qua1.x * qua2.x) + (qua1.y * qua2.y) + (qua1.z * qua2.z) + (qua1.w * qua2.w);
    bool flag = false;

    if (dot < 0.0f) {
        flag = true;
        dot = -dot;
    }

    if (dot > 0.999999f) {
        inverse = 1.0f - t;
        opposite = flag ? -t : t;
    } else {
        float ac = acosf(dot);
        float invSin = (1.0f / sinf(ac));

        inverse = (sinf((1.0f - t) * ac)) * invSin;
        opposite = flag ? ((-sinf(t * ac)) * invSin) : ((sinf(t * ac)) * invSin);
    }

    return Quaternion(
        (inverse * qua1.x) + (opposite * qua2.x),
        (inverse * qua1.y) + (opposite * qua2.y),
        (inverse * qua1.z) + (opposite * qua2.z),
        (inverse * qua1.w) + (opposite * qua2.w));
}

Quaternion Quaternion::lookRotation(const Vector3& forward_, const Vector3& up_)
{
    Vector3 forward = Vector3::normalize(forward_);
    Vector3 right = Vector3::normalize(Vector3::cross(up_, forward));
    Vector3 up = Vector3::cross(forward, right);
    float m00 = right.x;
    float m01 = right.y;
    float m02 = right.z;
    float m10 = up.x;
    float m11 = up.y;
    float m12 = up.z;
    float m20 = forward.x;
    float m21 = forward.y;
    float m22 = forward.z;

    Quaternion quaternion;

    float num8 = (m00 + m11) + m22;
    if (num8 > 0.f) {
        float fd = sqrtf(num8 + 1.0f);
        float hfd = 0.5f / fd;
        quaternion.w = fd * 0.5f;
        quaternion.x = (m12 - m21) * hfd;
        quaternion.y = (m20 - m02) * hfd;
        quaternion.z = (m01 - m10) * hfd;
        return quaternion;
    }
    if ((m00 >= m11) && (m00 >= m22)) {
        float fd = sqrtf(((1.0f + m00) - m11) - m22);
        float hfd = 0.5f / fd;
        quaternion.x = 0.5f * fd;
        quaternion.y = (m01 + m10) * hfd;
        quaternion.z = (m02 + m20) * hfd;
        quaternion.w = (m12 - m21) * hfd;
        return quaternion;
    }
    if (m11 > m22) {
        float fd = sqrtf(((1.0f + m11) - m00) - m22);
        float hfd = 0.5f / fd;
        quaternion.x = (m10 + m01) * hfd;
        quaternion.y = 0.5f * fd;
        quaternion.z = (m21 + m12) * hfd;
        quaternion.w = (m20 - m02) * hfd;
        return quaternion;
    }
    float fd = sqrtf(((1.0f + m22) - m00) - m11);
    float hfd = 0.5f / fd;
    quaternion.x = (m20 + m02) * hfd;
    quaternion.y = (m21 + m12) * hfd;
    quaternion.z = 0.5f * fd;
    quaternion.w = (m01 - m10) * hfd;
    return quaternion;
}

} // namespace ln
