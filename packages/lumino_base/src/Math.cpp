#include <lumino_base/Math.hpp>
#include <cmath>

namespace ln {

// ─── Quaternion ──────────────────────────────────────────────────────────

Quaternion Quaternion::fromAxisAngle(const Vector3& axis, f32 angle) {
    Vector3 n = axis.normalized();
    f32 half = angle * 0.5f;
    f32 s = std::sin(half);
    return {n.x * s, n.y * s, n.z * s, std::cos(half)};
}

Quaternion Quaternion::fromEuler(f32 pitch, f32 yaw, f32 roll) {
    f32 cp = std::cos(pitch * 0.5f), sp = std::sin(pitch * 0.5f);
    f32 cy = std::cos(yaw * 0.5f),   sy = std::sin(yaw * 0.5f);
    f32 cr = std::cos(roll * 0.5f),  sr = std::sin(roll * 0.5f);
    return {
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
        cr * cp * cy + sr * sp * sy,
    };
}

Quaternion Quaternion::slerp(const Quaternion& a, const Quaternion& b, f32 t) {
    f32 d = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    Quaternion b2 = b;
    if (d < 0) { b2 = {-b.x, -b.y, -b.z, -b.w}; d = -d; }
    if (d > 0.9995f) {
        // Linear interpolation for very close quaternions.
        return Quaternion{
            a.x + (b2.x - a.x) * t,
            a.y + (b2.y - a.y) * t,
            a.z + (b2.z - a.z) * t,
            a.w + (b2.w - a.w) * t,
        }.normalized();
    }
    f32 theta0 = std::acos(d);
    f32 theta = theta0 * t;
    f32 sinTheta = std::sin(theta);
    f32 sinTheta0 = std::sin(theta0);
    f32 s0 = std::cos(theta) - d * sinTheta / sinTheta0;
    f32 s1 = sinTheta / sinTheta0;
    return {
        a.x * s0 + b2.x * s1,
        a.y * s0 + b2.y * s1,
        a.z * s0 + b2.z * s1,
        a.w * s0 + b2.w * s1,
    };
}

// ─── Matrix4x4 ──────────────────────────────────────────────────────────

Matrix4x4 Matrix4x4::perspective(f32 fovY, f32 aspect, f32 nearZ, f32 farZ) {
    Matrix4x4 r;
    const f32 tanHalf = std::tan(fovY * 0.5f);
    for (auto& v : r.m) v = 0;
    r.m[0]  = 1.0f / (aspect * tanHalf);
    r.m[5]  = 1.0f / tanHalf;
    r.m[10] = farZ / (nearZ - farZ);
    r.m[11] = -1.0f;
    r.m[14] = (nearZ * farZ) / (nearZ - farZ);
    return r;
}

Matrix4x4 Matrix4x4::ortho(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ) {
    Matrix4x4 r;
    for (auto& v : r.m) v = 0;
    r.m[0]  = 2.0f / (right - left);
    r.m[5]  = 2.0f / (top - bottom);
    r.m[10] = 1.0f / (nearZ - farZ);
    r.m[12] = -(right + left) / (right - left);
    r.m[13] = -(top + bottom) / (top - bottom);
    r.m[14] = nearZ / (nearZ - farZ);
    r.m[15] = 1.0f;
    return r;
}

Matrix4x4 Matrix4x4::lookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
    Vector3 f = target - eye;
    const f32 fLen = f.length();
    if (fLen > 0) f = f * (1.0f / fLen);

    Vector3 s = Vector3::cross(f, up);
    const f32 sLen = s.length();
    if (sLen > 0) s = s * (1.0f / sLen);

    Vector3 u = Vector3::cross(s, f);

    Matrix4x4 r;
    r.m[0] = s.x;  r.m[4] = s.y;  r.m[8]  = s.z;  r.m[12] = -Vector3::dot(s, eye);
    r.m[1] = u.x;  r.m[5] = u.y;  r.m[9]  = u.z;  r.m[13] = -Vector3::dot(u, eye);
    r.m[2] = -f.x; r.m[6] = -f.y; r.m[10] = -f.z; r.m[14] = Vector3::dot(f, eye);
    r.m[3] = 0;    r.m[7] = 0;    r.m[11] = 0;    r.m[15] = 1;
    return r;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const {
    Matrix4x4 r;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            f32 sum = 0;
            for (int k = 0; k < 4; ++k) {
                sum += m[k * 4 + row] * rhs.m[col * 4 + k];
            }
            r.m[col * 4 + row] = sum;
        }
    }
    return r;
}

Matrix4x4 Matrix4x4::translate(const Vector3& t) {
    Matrix4x4 r;
    r.m[12] = t.x; r.m[13] = t.y; r.m[14] = t.z;
    return r;
}

Matrix4x4 Matrix4x4::scale(const Vector3& s) {
    Matrix4x4 r;
    r.m[0] = s.x; r.m[5] = s.y; r.m[10] = s.z;
    return r;
}

Matrix4x4 Matrix4x4::rotateX(f32 angle) {
    Matrix4x4 r;
    f32 c = std::cos(angle), s = std::sin(angle);
    r.m[5] = c; r.m[6] = s;
    r.m[9] = -s; r.m[10] = c;
    return r;
}

Matrix4x4 Matrix4x4::rotateY(f32 angle) {
    Matrix4x4 r;
    f32 c = std::cos(angle), s = std::sin(angle);
    r.m[0] = c; r.m[2] = -s;
    r.m[8] = s; r.m[10] = c;
    return r;
}

Matrix4x4 Matrix4x4::rotateZ(f32 angle) {
    Matrix4x4 r;
    f32 c = std::cos(angle), s = std::sin(angle);
    r.m[0] = c; r.m[1] = s;
    r.m[4] = -s; r.m[5] = c;
    return r;
}

Matrix4x4 Matrix4x4::rotateAxis(const Vector3& axis, f32 angle) {
    return fromQuaternion(Quaternion::fromAxisAngle(axis, angle));
}

Matrix4x4 Matrix4x4::fromQuaternion(const Quaternion& q) {
    f32 xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
    f32 xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
    f32 wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;

    Matrix4x4 r;
    r.m[0]  = 1 - 2 * (yy + zz);
    r.m[1]  = 2 * (xy + wz);
    r.m[2]  = 2 * (xz - wy);
    r.m[3]  = 0;
    r.m[4]  = 2 * (xy - wz);
    r.m[5]  = 1 - 2 * (xx + zz);
    r.m[6]  = 2 * (yz + wx);
    r.m[7]  = 0;
    r.m[8]  = 2 * (xz + wy);
    r.m[9]  = 2 * (yz - wx);
    r.m[10] = 1 - 2 * (xx + yy);
    r.m[11] = 0;
    r.m[12] = 0; r.m[13] = 0; r.m[14] = 0; r.m[15] = 1;
    return r;
}

Matrix4x4 Matrix4x4::transposed() const {
    Matrix4x4 r;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            r.m[j * 4 + i] = m[i * 4 + j];
    return r;
}

Matrix4x4 Matrix4x4::inversed() const {
    // Cofactor expansion (general 4x4 inverse).
    f32 a00 = m[0], a01 = m[1], a02 = m[2],  a03 = m[3];
    f32 a10 = m[4], a11 = m[5], a12 = m[6],  a13 = m[7];
    f32 a20 = m[8], a21 = m[9], a22 = m[10], a23 = m[11];
    f32 a30 = m[12], a31 = m[13], a32 = m[14], a33 = m[15];

    f32 b00 = a00 * a11 - a01 * a10;
    f32 b01 = a00 * a12 - a02 * a10;
    f32 b02 = a00 * a13 - a03 * a10;
    f32 b03 = a01 * a12 - a02 * a11;
    f32 b04 = a01 * a13 - a03 * a11;
    f32 b05 = a02 * a13 - a03 * a12;
    f32 b06 = a20 * a31 - a21 * a30;
    f32 b07 = a20 * a32 - a22 * a30;
    f32 b08 = a20 * a33 - a23 * a30;
    f32 b09 = a21 * a32 - a22 * a31;
    f32 b10 = a21 * a33 - a23 * a31;
    f32 b11 = a22 * a33 - a23 * a32;

    f32 det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;
    if (std::abs(det) < 1e-12f) return identity();
    f32 invDet = 1.0f / det;

    Matrix4x4 r;
    r.m[0]  = ( a11 * b11 - a12 * b10 + a13 * b09) * invDet;
    r.m[1]  = (-a01 * b11 + a02 * b10 - a03 * b09) * invDet;
    r.m[2]  = ( a31 * b05 - a32 * b04 + a33 * b03) * invDet;
    r.m[3]  = (-a21 * b05 + a22 * b04 - a23 * b03) * invDet;
    r.m[4]  = (-a10 * b11 + a12 * b08 - a13 * b07) * invDet;
    r.m[5]  = ( a00 * b11 - a02 * b08 + a03 * b07) * invDet;
    r.m[6]  = (-a30 * b05 + a32 * b02 - a33 * b01) * invDet;
    r.m[7]  = ( a20 * b05 - a22 * b02 + a23 * b01) * invDet;
    r.m[8]  = ( a10 * b10 - a11 * b08 + a13 * b06) * invDet;
    r.m[9]  = (-a00 * b10 + a01 * b08 - a03 * b06) * invDet;
    r.m[10] = ( a30 * b04 - a31 * b02 + a33 * b00) * invDet;
    r.m[11] = (-a20 * b04 + a21 * b02 - a23 * b00) * invDet;
    r.m[12] = (-a10 * b09 + a11 * b07 - a12 * b06) * invDet;
    r.m[13] = ( a00 * b09 - a01 * b07 + a02 * b06) * invDet;
    r.m[14] = (-a30 * b03 + a31 * b01 - a32 * b00) * invDet;
    r.m[15] = ( a20 * b03 - a21 * b01 + a22 * b00) * invDet;
    return r;
}

} // namespace ln
