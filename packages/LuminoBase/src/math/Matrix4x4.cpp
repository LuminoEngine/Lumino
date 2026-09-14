#include <LuminoBase/Logger.hpp>
#include <LuminoBase/math/Math.hpp>
#include <LuminoBase/math/Matrix4x4.hpp>

namespace ln {

namespace {
// 軸ベクトルが縮退しているかどうかの判定に使う長さの下限。
constexpr float kAxisEpsilon = 1e-6f;
} // namespace

Matrix4x4::Matrix4x4(
    float x0, float y0, float z0, float w0,
    float x1, float y1, float z1, float w1,
    float x2, float y2, float z2, float w2,
    float x3, float y3, float z3, float w3)
    : m{
        x0, y0, z0, w0,
        x1, y1, z1, w1,
        x2, y2, z2, w2,
        x3, y3, z3, w3
    } {
}

// クリップ空間の Z を 0..1 とする右手系の射影 (perspectiveRH_ZO)。
// WebGPU、Vulkan、D3D と同じ規約であり、docs/graphics-conventions.md と ortho() に整合する。
// 結果は glm::perspectiveRH_ZO と一致する。
Matrix4x4 Matrix4x4::perspectiveRH(float fovY, float aspect, float nearZ, float farZ) {
    Matrix4x4 r;
    const float tanHalf = std::tan(fovY * 0.5f);
    for (auto& v : r.m) {
        v = 0;
    }
    r.m[0]  = 1.0f / (aspect * tanHalf);
    r.m[5]  = 1.0f / tanHalf;
    r.m[10] = farZ / (nearZ - farZ);
    r.m[11] = -1.0f;
    r.m[14] = -(farZ * nearZ) / (farZ - nearZ);
    return r;
}

Matrix4x4 Matrix4x4::ortho(float left, float right, float bottom, float top, float nearZ, float farZ) {
    Matrix4x4 r;
    for (auto& v : r.m) {
        v = 0;
    }
    r.m[0]  = 2.0f / (right - left);
    r.m[5]  = 2.0f / (top - bottom);
    r.m[10] = 1.0f / (nearZ - farZ);
    r.m[12] = -(right + left) / (right - left);
    r.m[13] = -(top + bottom) / (top - bottom);
    r.m[14] = nearZ / (nearZ - farZ);
    r.m[15] = 1.0f;
    return r;
}

Matrix4x4 Matrix4x4::lookAtRH(const Vector3& position, const Vector3& lookAt_, const Vector3& up) {
    Vector3 xaxis, yaxis;
    // カメラ位置から注視点へ向かうベクトルを Z 軸とする
    Vector3 zaxis = lookAt_ - position;
    zaxis.normalize();
    // Z 軸と上方向のベクトルの外積をとると X 軸が求まる
    xaxis = Vector3::cross(zaxis, up);
    // 視点と注視点が同じ場合は zaxis が (normalized() は長さ 0 でゼロベクトルを返す)、
    // 上方向が視線と平行な場合は外積がゼロになる。そのまま進めると全頂点を 1 点に潰す
    // ゼロ行列ができてしまうため、単位行列にフォールバックする。
    if (xaxis.length() <= kAxisEpsilon) {
        LN_LOG_WARNING("Matrix4x4::lookAtRH: 視線方向と上方向が縮退しているため、単位行列を返します。");
        return identity();
    }
    xaxis.normalize();
    // 2 つの軸が求まったので、その外積が残りの軸 (Y 軸) になる
    yaxis = Vector3::cross(xaxis, zaxis);

    return Matrix4x4(
        xaxis.x, yaxis.x, -zaxis.x, 0.0f, 
        xaxis.y, yaxis.y, -zaxis.y, 0.0f,
        xaxis.z, yaxis.z, -zaxis.z, 0.0f,
        -(xaxis.x * position.x + xaxis.y * position.y + xaxis.z * position.z),
        -(yaxis.x * position.x + yaxis.y * position.y + yaxis.z * position.z),
        (zaxis.x * position.x + zaxis.y * position.y + zaxis.z * position.z),
        1.0f);
}

Vector3 Matrix4x4::transformCoord(const Vector3& v) const {
    // 列優先 (m[col*4 + row])、列ベクトル規約 out = M * (v, 1)。平行移動は m[12..14]。
    return Vector3{
        m[0] * v.x + m[4] * v.y + m[8]  * v.z + m[12],
        m[1] * v.x + m[5] * v.y + m[9]  * v.z + m[13],
        m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14],
    };
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const {
    Matrix4x4 r;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            float sum = 0;
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

Matrix4x4 Matrix4x4::rotateX(float angle) {
    Matrix4x4 r;
    float c = std::cos(angle), s = std::sin(angle);
    r.m[5] = c; r.m[6] = s;
    r.m[9] = -s; r.m[10] = c;
    return r;
}

Matrix4x4 Matrix4x4::rotateY(float angle) {
    Matrix4x4 r;
    float c = std::cos(angle), s = std::sin(angle);
    r.m[0] = c; r.m[2] = -s;
    r.m[8] = s; r.m[10] = c;
    return r;
}

Matrix4x4 Matrix4x4::rotateZ(float angle) {
    Matrix4x4 r;
    float c = std::cos(angle), s = std::sin(angle);
    r.m[0] = c; r.m[1] = s;
    r.m[4] = -s; r.m[5] = c;
    return r;
}

Matrix4x4 Matrix4x4::rotateAxis(const Vector3& axis, float angle) {
    return fromQuaternion(Quaternion::fromAxisAngle(axis, angle));
}

Matrix4x4 Matrix4x4::fromQuaternion(const Quaternion& q) {
    float xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
    float xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
    float wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;

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
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            r.m[j * 4 + i] = m[i * 4 + j];
        }
    }
    return r;
}

Matrix4x4 Matrix4x4::inversed() const {
    // 余因子展開による一般的な 4x4 逆行列。
    float a00 = m[0], a01 = m[1], a02 = m[2],  a03 = m[3];
    float a10 = m[4], a11 = m[5], a12 = m[6],  a13 = m[7];
    float a20 = m[8], a21 = m[9], a22 = m[10], a23 = m[11];
    float a30 = m[12], a31 = m[13], a32 = m[14], a33 = m[15];

    float b00 = a00 * a11 - a01 * a10;
    float b01 = a00 * a12 - a02 * a10;
    float b02 = a00 * a13 - a03 * a10;
    float b03 = a01 * a12 - a02 * a11;
    float b04 = a01 * a13 - a03 * a11;
    float b05 = a02 * a13 - a03 * a12;
    float b06 = a20 * a31 - a21 * a30;
    float b07 = a20 * a32 - a22 * a30;
    float b08 = a20 * a33 - a23 * a30;
    float b09 = a21 * a32 - a22 * a31;
    float b10 = a21 * a33 - a23 * a31;
    float b11 = a22 * a33 - a23 * a32;

    float det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

    // det はスケールの 4 乗で変化するため、固定の絶対値をしきい値にすると小スケールの
    // 可逆な行列まで特異と誤判定する (例: scale(0.0001) の det は 1e-12)。
    // 代わりに逆数が有効な有限値になるかどうかだけを見る (0 なら inf、inf なら 0 になる)。
    const float invDet = 1.0f / det;
    if (!std::isfinite(invDet) || invDet == 0.0f) {
        LN_LOG_WARNING("Matrix4x4::inversed: 逆行列を計算できないため、単位行列を返します。 (det=%g)", static_cast<double>(det));
        return identity();
    }

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
