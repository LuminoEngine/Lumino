#include <LuminoCore/graphics/Camera.hpp>
#include <cmath>

namespace ln {

Camera::Camera() = default;

void Camera::setPerspective(f32 fovY, f32 aspect, f32 nearClip, f32 farClip) {
    m_mode = ProjectionMode::Perspective;
    m_fovY = fovY;
    m_aspect = aspect;
    m_nearClip = nearClip;
    m_farClip = farClip;
}

void Camera::setOrthographic(f32 width, f32 height, f32 nearClip, f32 farClip) {
    m_mode = ProjectionMode::Orthographic;
    m_orthoWidth = width;
    m_orthoHeight = height;
    m_nearClip = nearClip;
    m_farClip = farClip;
}

void Camera::setLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
    m_position = eye;
    m_target = target;
    m_up = up;
}

void Camera::setOrbit(const Vector3& target, f32 distance, f32 yaw, f32 pitch) {
    m_target = target;
    f32 cp = std::cos(pitch);
    m_position = {
        target.x + distance * cp * std::sin(yaw),
        target.y + distance * std::sin(pitch),
        target.z + distance * cp * std::cos(yaw),
    };
    m_up = Vector3::unitY();
}

Matrix4x4 Camera::viewMatrix() const {
    return Matrix4x4::lookAtRH(m_position, m_target, m_up);
}

Matrix4x4 Camera::projectionMatrix() const {
    if (m_mode == ProjectionMode::Perspective) {
        return Matrix4x4::perspectiveRH(m_fovY, m_aspect, m_nearClip, m_farClip);
    } else {
        f32 hw = m_orthoWidth * 0.5f;
        f32 hh = m_orthoHeight * 0.5f;
        return Matrix4x4::ortho(-hw, hw, -hh, hh, m_nearClip, m_farClip);
    }
}

Matrix4x4 Camera::viewProjectionMatrix() const {
    return projectionMatrix() * viewMatrix();
}

} // namespace ln
