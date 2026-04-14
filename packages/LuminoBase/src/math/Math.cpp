#include <LuminoBase/math/Math.hpp>

namespace ln {

// ─── Quaternion ──────────────────────────────────────────────────────────

Quaternion Quaternion::fromAxisAngle(const Vector3& axis, float angle) {
    Vector3 n = axis.normalized();
    float half = angle * 0.5f;
    float s = std::sin(half);
    return {n.x * s, n.y * s, n.z * s, std::cos(half)};
}

Quaternion Quaternion::fromEuler(float pitch, float yaw, float roll) {
    float cp = std::cos(pitch * 0.5f), sp = std::sin(pitch * 0.5f);
    float cy = std::cos(yaw * 0.5f),   sy = std::sin(yaw * 0.5f);
    float cr = std::cos(roll * 0.5f),  sr = std::sin(roll * 0.5f);
    return {
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
        cr * cp * cy + sr * sp * sy,
    };
}

Quaternion Quaternion::slerp(const Quaternion& a, const Quaternion& b, float t) {
    float d = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
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
    float theta0 = std::acos(d);
    float theta = theta0 * t;
    float sinTheta = std::sin(theta);
    float sinTheta0 = std::sin(theta0);
    float s0 = std::cos(theta) - d * sinTheta / sinTheta0;
    float s1 = sinTheta / sinTheta0;
    return {
        a.x * s0 + b2.x * s1,
        a.y * s0 + b2.y * s1,
        a.z * s0 + b2.z * s1,
        a.w * s0 + b2.w * s1,
    };
}

} // namespace ln
