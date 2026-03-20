#include <lumino_base/Math.hpp>
#include <cmath>

namespace lumino {

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

} // namespace lumino
