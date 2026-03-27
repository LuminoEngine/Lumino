#include <lumino_base/math/Math.hpp>

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

} // namespace ln
